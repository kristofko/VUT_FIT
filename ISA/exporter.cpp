/**
 * @file exporter.cpp
 * @author Kristof Siska, xsiska16
 *
 * Last update : Oct 2022
 *
 * Functions inspired by echo-udp-client2, by (c) Petr Matousek, 2016
 */
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include "exporter.h"
#include "flow.h"
#define BUFFER 256

u_int32_t count_flow = 0;


// buffer length

void export_flows(std::map<flow_key_t, flow> &my_map, timeval current_time, args_t args, timeval boot){
	int sock = get_connected_sock(args); // socket descriptor
	int size_msg, i = 0;

	export_flow_t export_flow;
	u_int32_t curr_time_ms = get_uptime(current_time, boot);

	flow_header_t flow_header;
	flow_header.version = 5;
	flow_header.count = 1;
	flow_header.sys_uptime = get_uptime(current_time, boot);
	flow_header.unix_secs = current_time.tv_sec;
	flow_header.unix_nsecs = current_time.tv_usec * 1000;

	export_flow.nfthdr = flow_header;

	export_flow = htons_header(export_flow);

	for (std::map<flow_key_t, flow>::iterator iter = my_map.begin(); iter != my_map.end();)
	{
		export_flow.payload = iter->second;
		// Check for active

		if ((long)(curr_time_ms - export_flow.payload.first) >= args.active * 1000)
		{
			export_flow.nfthdr.flow_sequence = htonl(count_flow);
			count_flow++;
			export_flow = htons_payload(export_flow);
			send_flow(sock, export_flow);
			my_map.erase(iter++);
		}

		// Check for inactive
		else if (curr_time_ms - export_flow.payload.last >= args.inactive * 1000)
		{
			export_flow.nfthdr.flow_sequence = htonl(count_flow);
			count_flow++;
			export_flow = htons_payload(export_flow);
			send_flow(sock, export_flow);
			my_map.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	if (my_map.size() >= args.count){
		flow_key_t key = find_oldest_flow_key(my_map);
		export_flow.payload = my_map[key];
		export_flow.nfthdr.flow_sequence = htonl(count_flow);
		count_flow++;
		export_flow = htons_payload(export_flow);
		send_flow(sock, export_flow);
		my_map.erase(key);
	}

	close(sock);
}

int get_port(args_t args)
{
	char port[10];
	memset(port, 0, sizeof(port));
	int i, n = 0;
	bool append = false;
	for (; i < strlen(args.collector); i++)
	{
		if (append)
		{
			port[n] = args.collector[i];
			n++;
		}
		if (args.collector[i] == ':')
		{
			append = true;
		}
	}
	port[n] = '\0';

	if (append){
		return atoi(port);
	}
	else {
		return 2055;
	}

}

void get_host_or_ip(args_t args, char *host_name){
	char host[256];
	memset(host, 0, sizeof(host));
	int n = 0;
	for (int i = 0; i < strlen(args.collector) && args.collector[i] != ':'; i++)
	{
		host[n] = args.collector[i];
		n++;
	}
	host[n] = '\0';
	strcpy(host_name, host);

	return;
}

export_flow_t htons_header(export_flow_t exp_flow){

	export_flow_t curr_flow = exp_flow;
	curr_flow.nfthdr.version = htons(5);
	curr_flow.nfthdr.count = htons(1);
	curr_flow.nfthdr.sys_uptime = htonl(curr_flow.nfthdr.sys_uptime);
	curr_flow.nfthdr.unix_secs = htonl(curr_flow.nfthdr.unix_secs);
	curr_flow.nfthdr.unix_nsecs = htonl(curr_flow.nfthdr.unix_nsecs);
	curr_flow.nfthdr.sampling_interval = htons(curr_flow.nfthdr.sampling_interval);

	return curr_flow;
	
}

export_flow_t htons_payload(export_flow_t exp_flow){

	export_flow_t curr_flow = exp_flow;

	curr_flow.payload.first = htonl(curr_flow.payload.first);
	curr_flow.payload.last = htonl(curr_flow.payload.last);
	curr_flow.payload.dPkts = htonl(curr_flow.payload.dPkts);
	curr_flow.payload.dOctets = htonl(curr_flow.payload.dOctets);



	return curr_flow;
}

void send_flow(int sock, export_flow_t export_flow){
	char buffer[BUFFER] = "";
	char *ptr = &buffer[0];
	memcpy(ptr, &export_flow.nfthdr.version, sizeof(struct flow_header_t));
	ptr += sizeof(struct flow_header_t);
	memcpy(ptr, &export_flow.payload, sizeof(struct flow));

	int i = send(sock, buffer, sizeof(struct flow) + sizeof(struct flow_header_t), 0); // send data to the server
	if (i == -1)																	   // check if data was sent correctly
		err(1, "send() failed");
	else if (i != sizeof(struct flow) + sizeof(struct flow_header_t))
		err(1, "send(): buffer written partially %d %d", i, sizeof(struct flow) + sizeof(struct flow_header_t));

	return;
}

void export_all_netflows(std::map<flow_key_t, flow> &my_map, timeval current_time, timeval boot_time, args_t args){
	auto curr_time = get_uptime(current_time, boot_time);
	export_flow_t export_flow;
	flow_header_t flow_header;
	flow_header.sys_uptime = get_uptime(current_time, boot_time);
	flow_header.unix_secs = current_time.tv_sec;
	flow_header.unix_nsecs = current_time.tv_usec * 1000;
	export_flow.nfthdr = flow_header;

	export_flow = htons_header(export_flow);


	int sock = get_connected_sock(args); 

	for (std::map<flow_key_t, flow>::iterator iter = my_map.begin(); iter != my_map.end();){

		export_flow.payload = iter->second;
		export_flow.nfthdr.flow_sequence = htonl(count_flow);
		count_flow++;
		
		export_flow = htons_payload(export_flow);
		send_flow(sock, export_flow);
		my_map.erase(iter++);
	}
}

int get_connected_sock(args_t args){
	int sock;
	struct sockaddr_in server; // address structures of the server and the client
	struct hostent *servent;   // network host entry required by gethostbyname()
	char buffer[BUFFER] = "";
	char host_name[BUFFER];

	memset(&server, 0, sizeof(server)); // erase the server structure
	server.sin_family = AF_INET;

	get_host_or_ip(args, host_name);
	int port = get_port(args);

	in_addr_t addr = inet_addr(host_name);

	if ((servent = gethostbyname(host_name)) == NULL) // check the first parameter
		errx(1,"gethostbyname() failed\n");


	// copy the first parameter to the server.sin_addr structure
	memcpy(&server.sin_addr,servent->h_addr,servent->h_length); 
 	// memcpy(&server.sin_addr,&addr, sizeof(addr)); 
	server.sin_port = htons(port);

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) // create a client socket
		err(1, "socket() failed\n");

	// create a connected UDP socket
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
		err(1, "connect() failed");

	return sock;
}

flow_key_t find_oldest_flow_key(std::map<flow_key_t, flow> &my_map){
	u_int32_t oldest_flow = my_map.begin()->second.first; // Uptime of first received packet in flow
	flow_key_t oldest_key = my_map.begin()->first;
	for (std::map<flow_key_t, flow>::iterator iter = my_map.begin(); iter != my_map.end(); iter++){
		if (iter->second.first < oldest_flow){
			oldest_key = iter->first;
		}
	}

	return oldest_key;
}