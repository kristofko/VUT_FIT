/**
 * File : hinfosvc.cpp
 * Author : Kristof Siska (xsiska16)
 * Date : 15.2.2022
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <unistd.h>

float get_current_load();	

int main(int argc, char **argv) {
	
	if (argc != 2){
		perror("Wrong number of arguments");
		return -1;
	}	

	char line[256] = {0};
	char model_name[256] = {0};

	int port_number = atoi(argv[1]);
	int server_socket;
	int optval = 1;


	// Get cpu info
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");

	if (cpuinfo == NULL){
		perror("Failed to open /proc/cpuinfo");
		return EXIT_FAILURE;
	}

	while (fgets(line, 256, cpuinfo) != NULL){
		if (!strncmp(line, "model name", strlen("model name"))){
			strcpy(model_name, &(line[13]));
			break;
		}
	}
	
	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (server_socket <= 0){
		perror("Socket error ");
		return EXIT_FAILURE;
	}

	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval));

	sockaddr_in sock_address, client_address;

	memset(&sock_address, 0, sizeof(sock_address));
	sock_address.sin_family = AF_INET;
	sock_address.sin_port = htons(port_number);
	sock_address.sin_addr.s_addr = INADDR_ANY;
	
	// Bind to address
	if (bind(server_socket, (struct sockaddr *) &sock_address, sizeof(sock_address)) < 0){
		perror("Bind error ");
		return EXIT_FAILURE;
	}

	int client_socket;
	int address_len;
	int message_len;
	char buffer[200] = {0};
	address_len = sizeof(client_address);
	listen(server_socket, 3);

	char ok_message[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
	char answer[256] = {0};

	while(1){

		client_socket = accept(server_socket, (struct sockaddr *) &client_address,(unsigned int *) &address_len);

		if (client_socket < 0){
			perror("Accept ");
			return EXIT_FAILURE;
		}

		memset(buffer, 0, 200);
		memset(answer, 0, 256);

		message_len = recv(client_socket, buffer, 200, 0);


		// Handle hostname request
		if (!strncmp("GET /hostname", buffer, strlen("GET /hostname"))) {
			gethostname(buffer, 200);
			strcpy(answer, ok_message);
			strcat(answer, buffer);
			strcat(answer, "\n");

			send(client_socket, answer, strlen(answer), 0);
			close(client_socket);
			
		}

		// Handle cpu-name request
		else if (!strncmp("GET /cpu-name", buffer, strlen("GET /cpu-name"))){
			strcpy(answer, ok_message);
			strcat(answer, model_name);
		
			send(client_socket, answer, strlen(answer), 0);
			close(client_socket);
		}


		// Handle load request
		else if (!strncmp("GET /load", buffer, strlen("GET /load"))){
			float cpu = get_current_load();
			cpu *= 100;
			strcpy(answer, ok_message);
			sprintf(answer, "%s%.2f%%\n", ok_message, cpu);

			send(client_socket, answer, strlen(answer), 0);
			close(client_socket);
		}

		else {
			send(client_socket, "HTTP/1.0 400 Bad Request", strlen("400 Bad Request"), 0);
			close(client_socket);
		}
	 }

	return EXIT_SUCCESS;
}


float get_current_load(){
	FILE *f = fopen("/proc/stat", "rb");

	if (f == NULL){
		perror("Cannot open proc/stat");
		return EXIT_FAILURE;
	}

	int cpu;

	char line[512];
	char name[15];

		/* 
		From: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
		PrevIdle = previdle + previowait
			Idle = idle + iowait

			PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
			NonIdle = user + nice + system + irq + softirq + steal

			PrevTotal = PrevIdle + PrevNonIdle
			Total = Idle + NonIdle

			totald = Total - PrevTotal
			idled = Idle - PrevIdle

		CPU_Percentage = (totald - idled)/totald
 */
	long long unsigned prev_user, prev_nice, prev_system, prev_idle, prev_iowait, prev_irq, prev_softirq, prev_steal, prev_quest;
	long long unsigned user, nice, system, idle, iowait, irq, softirq, steal;

	if (fgets(line, 512, f) == NULL){
		perror("Internal fgets error");
		return EXIT_FAILURE;
	}

	sscanf(line, "%s %llu %llu %llu %llu %llu %llu %llu %llu", name, &prev_user, &prev_nice, &prev_system, &prev_idle, &prev_iowait, &prev_irq, &prev_softirq, &prev_steal );
	
	fclose(f);

	sleep(1);

	f = fopen("/proc/stat", "rb");

	if (f == NULL){
		perror("Cannot open proc/stat");
		return EXIT_FAILURE;
	}

	
	if (fgets(line, 512, f) == NULL){
		perror("Internal fgets error");
		return EXIT_FAILURE;
	}

	sscanf(line, "%s %llu %llu %llu %llu %llu %llu %llu %llu", name, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal );
	
	long long unsigned PrevIdle = prev_idle + prev_iowait;
	long long unsigned Idle = idle + iowait;
	long long unsigned PrevNonIdle =  prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
	long long unsigned NonIdle = user + nice + system + irq + softirq + steal;
	long long unsigned PrevTotal = PrevIdle + PrevNonIdle;
	long long unsigned Total = Idle + NonIdle;
	
	long int totald = Total - PrevTotal;

	long int idled = Idle - PrevIdle;

	float CPU = ((float) totald - (float) idled) /  (float)totald;
	
	return CPU;
}