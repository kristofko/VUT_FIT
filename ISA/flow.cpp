/**
 * @file flow.cpp
 * @author  Krištof Šiška - xsiska16, FIT
 * 
 * Last update : Nov 2022
 *
 * Solution to homework ISA - netflow
 * Compile with -lpcap flag
 * Compiled : g++ version 9.4.0
 */
#include <unistd.h>
#include "flow.h"
#include "arg_parser.h"
#include "exporter.h"

extern u_int32_t count_flow;

int main (int argc, char **argv){
	args_t args;
	struct flow my_flow;

	check_args(argc, argv, &args);

	pcap_t *pcap_handle;

	std::map< flow_key_t ,struct flow> my_map;
	
	char errbuf[PCAP_ERRBUF_SIZE];  // constant defined in pcap.h
	const u_char *packet;
	struct ip *my_ip;
	struct ip6 *my_ip6;
	const struct tcphdr *my_tcp;    // pointer to the beginning of TCP header
	const struct udphdr *my_udp;    // pointer to the beginning of UDP header
	struct pcap_pkthdr header;  
	struct ether_header *eptr;
	u_int size_ip;
	char stdin_filename[] = "-";
	char dst_ip[INET_ADDRSTRLEN];
	char src_ip[INET_ADDRSTRLEN];
	timeval boot_time = {-1,-1};

	if (args.filename ==  nullptr ){
		args.filename = stdin_filename;
	}

 	if ( (pcap_handle = pcap_open_offline(args.filename, errbuf)) == NULL){
		fprintf(stderr, "Cannot open file %s for reading\n", args.filename);
		return EXIT_FAILURE;
	}

	while((packet = pcap_next(pcap_handle, &header)) != NULL){
		eptr = (struct ether_header *) packet;
		
		if (boot_time.tv_sec == -1){
			boot_time.tv_sec = header.ts.tv_sec;
			boot_time.tv_usec = header.ts.tv_usec;
		}
	

		switch(ntohs(eptr->ether_type)){
			case ETHERTYPE_IP:
				my_ip =  (struct ip * )(packet + SIZE_ETHERNET );
				size_ip = my_ip->ip_hl*4;
				export_flows(my_map, header.ts, args, boot_time);
				switch(my_ip->ip_p){
					case 6:
					{   // TCP Protocol
						my_tcp = (struct tcphdr *)(packet + SIZE_ETHERNET + size_ip);

						// Create or update netflow
						// We want to create a Key to the map
						// The key is a tuple containing : 
						// IP src, IP dst, port src, port dst, protocol
						strcpy(dst_ip,inet_ntoa(my_ip->ip_dst)); 
						strcpy(src_ip,inet_ntoa(my_ip->ip_src));
						flow_key_t my_key = 
							{dst_ip,src_ip, my_tcp->th_sport, my_tcp->th_dport, 6} ;
						
						if (my_map.find(my_key) == my_map.end()){
							// key is not in map, so we insert new flow
							my_flow.srcaddr = my_ip->ip_src;
							my_flow.dstaddr = my_ip->ip_dst;
							my_flow.dPkts = 1;
							my_flow.first = get_uptime(header.ts, boot_time);
							my_flow.last = get_uptime(header.ts, boot_time);
							my_flow.srcport = my_tcp->th_sport;
							my_flow.dstport = my_tcp->th_dport;
							my_flow.dOctets = ntohs(my_ip->ip_len);
							my_flow.prot = 6;
							my_flow.tos = my_ip->ip_tos;
							my_flow.tcp_flags = my_tcp->th_flags;
							my_map.insert({my_key, my_flow});
							if (my_tcp->th_flags & TH_FIN || my_tcp->th_flags & TH_RST){
								int sock = get_connected_sock(args);
								flow_header_t flow_header;
								export_flow_t export_flow;
								u_int32_t curr_time_ms = get_uptime(header.ts, boot_time);

								flow_header.version = 5;
								flow_header.count = 1;
								flow_header.sys_uptime = get_uptime(header.ts, boot_time);
								flow_header.unix_secs = header.ts.tv_sec;
								flow_header.unix_nsecs = header.ts.tv_usec * 1000;

								export_flow.nfthdr = flow_header;
								export_flow.payload = my_map[my_key];

								export_flow = htons_header(export_flow);
								export_flow.nfthdr.flow_sequence = htonl(count_flow);
								count_flow++;
								export_flow = htons_payload(export_flow);
								send_flow(sock, export_flow);
								my_map.erase(my_key);

								close(sock);

								break;
								}
							
								
						
							
							
						}

						else {
							// Flow already exists, updating flow
							flow update_flow = my_map[my_key];
							
								update_flow.tcp_flags = (update_flow.tcp_flags | my_tcp->th_flags);
								update_flow.dPkts++;
								update_flow.dOctets += ntohs(my_ip->ip_len);
								update_flow.last = get_uptime(header.ts, boot_time);
								my_map[my_key] = update_flow;
						
							if (my_tcp->th_flags & TH_FIN || my_tcp->th_flags & TH_RST){
								my_map.insert({my_key, my_flow});
								int sock = get_connected_sock(args);
								flow_header_t flow_header;
								export_flow_t export_flow;
								u_int32_t curr_time_ms = get_uptime(header.ts, boot_time);
								flow_header.version = 5;
								flow_header.count = 1;
								flow_header.sys_uptime = get_uptime(header.ts, boot_time);
								flow_header.unix_secs = header.ts.tv_sec;
								flow_header.unix_nsecs = header.ts.tv_usec * 1000;

								export_flow.nfthdr = flow_header;
								export_flow.payload = my_map[my_key];

								export_flow = htons_header(export_flow);
								export_flow.nfthdr.flow_sequence = htonl(count_flow);
								count_flow++;
								export_flow = htons_payload(export_flow);
								send_flow(sock, export_flow);
								my_map.erase(my_key);

								close(sock);
								
								} 
						}
						break;
					}

					case 17:
					{
						// UDP Protocol
						my_udp = (struct udphdr *)(packet + SIZE_ETHERNET + size_ip);
						// Create or update netflow
						strcpy(dst_ip,inet_ntoa(my_ip->ip_dst)); 
						strcpy(src_ip,inet_ntoa(my_ip->ip_src));
						flow_key_t my_key = {dst_ip,src_ip, my_udp->uh_sport, my_udp->uh_dport, 17};
						
						
						if (my_map.find(my_key) == my_map.end()){
							// key is not in map, so we insert new flow

							my_flow.srcaddr = my_ip->ip_src;
							my_flow.dstaddr = my_ip->ip_dst;
							my_flow.dPkts = 1;
							my_flow.first = get_uptime(header.ts, boot_time);
							my_flow.last = get_uptime(header.ts, boot_time);
							my_flow.srcport = my_udp->uh_sport;
							my_flow.dstport = my_udp->uh_dport;
							my_flow.dOctets = ntohs(my_ip->ip_len);
							
							my_flow.tcp_flags = 0;
							my_flow.prot = 17;
							my_flow.tos = my_ip->ip_tos;
							my_map.insert({my_key, my_flow});
							}

						else {
							// Flow already exists, updating flow
							flow update_flow = my_map[my_key];
							update_flow.dPkts++;
							update_flow.dOctets += ntohs(my_ip->ip_len);
							update_flow.last = get_uptime(header.ts, boot_time);
							my_map[my_key] = update_flow;
							}
						break;
						}

						case 1:
							// ICMP protocol
							// Create or update netflow
							strcpy(dst_ip,inet_ntoa(my_ip->ip_dst)); 
							strcpy(src_ip,inet_ntoa(my_ip->ip_src));
							flow_key_t my_key = {dst_ip,src_ip, 0, 0, 1} ;
							
							if (my_map.find(my_key) == my_map.end()){
								// key is not in map, so we insert new flow
								my_flow.srcaddr = my_ip->ip_src;
								my_flow.dstaddr = my_ip->ip_dst;
								my_flow.dPkts = 1;
								my_flow.first = get_uptime(header.ts, boot_time);
								my_flow.last = get_uptime(header.ts, boot_time);
								my_flow.srcport = 0;
								my_flow.dstport = 0;
								my_flow.dOctets = ntohs(my_ip->ip_len);
								
								my_flow.tcp_flags = 0;
								my_flow.prot = 1;
								my_flow.tos = my_ip->ip_tos;
								my_map.insert({my_key, my_flow});
							}

							else {
								// Flow already exists, updating flow
								flow update_flow = my_map[my_key];
								update_flow.dPkts++;
								update_flow.dOctets += ntohs(my_ip->ip_len);
								update_flow.last = get_uptime(header.ts, boot_time);
								my_map[my_key] = update_flow;
							}

							break;
				}

			case ETHERTYPE_IPV6:
				break;
			default: break;
		}
	}

	export_all_netflows(my_map, header.ts, boot_time, args);

	return 0;
}


u_int32_t get_uptime(timeval current, timeval boot){
	long uptime_s = current.tv_sec - boot.tv_sec;
	long uptime_usec = current.tv_usec - boot.tv_usec;

	uptime_s *= 1000;
	uptime_usec = uptime_usec / 1000;
	return (u_int32_t)(uptime_s + uptime_usec); 
}

void print_flow(flow my_flow){
	printf("DST : %s\n", inet_ntoa(my_flow.dstaddr));
	printf("SRC : %s\n", inet_ntoa(my_flow.srcaddr));
	printf("dPkts : %d\n", my_flow.dPkts);
	printf("dOctets : %d\n", my_flow.dOctets);
	printf("flags : %d\n", my_flow.tcp_flags);
	printf("port src : %d\n", my_flow.srcport);
	printf("port dst : %d\n", my_flow.dstport);
	printf("First : %d\n", my_flow.first);
	printf("Last : %d\n", my_flow.last);
	printf("END FLOW\n\n");
}