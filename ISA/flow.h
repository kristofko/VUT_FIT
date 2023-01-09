/**
 * @file flow.h
 * @author  Krištof Šiška - xsiska16, FIT
 * 
 * Last update : Nov 2022
 *
 */

#ifndef FLOW_H
#define FLOW_H

#include <iostream>
#include <tuple>
#include <map>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define __FAVOR_BSD
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/ether.h> 
#include <netinet/if_ether.h> 
#include <err.h>
#include <netinet/ip_icmp.h>

#define SIZE_ETHERNET 14

#define TCP_FIN 6
#define TCP_RST 4


typedef struct flow_header_t
{
	u_int16_t version = 5;
	u_int16_t count = 1;
	u_int32_t sys_uptime;
	u_int32_t unix_secs;
	u_int32_t unix_nsecs;
	u_int32_t flow_sequence;
	u_int8_t engine_type = 0;
	u_int8_t engine_id = 0;
	u_int16_t sampling_interval = 0;

} flow_header_t;

typedef struct flow
{
	in_addr srcaddr;		 /* IP src */
	in_addr dstaddr;		 /* IP dst */	
	u_int32_t nexthop = 0;
	u_int16_t input = 0;
	u_int16_t output = 0;
	u_int32_t dPkts;		 /* Num of packets in flow */
	u_int32_t dOctets;		 /* NUm of */
	u_int32_t first;		 /* Time of first packet from uptime */
	u_int32_t last;			 /* Time of last packet */
	u_int16_t srcport;		 /* PORT src */
	u_int16_t dstport;		 /* PORT dst */
	u_int8_t  pad1 = 0;
	u_int8_t  tcp_flags;	 /* Cumultative tcp flags */
	u_int8_t  prot;			 /* Number of protocol */
	u_int8_t  tos;			 /* ToS */
	u_int16_t src_as = 0;
	u_int16_t dst_as = 0;
	u_int8_t  src_mask = 0;
	u_int8_t  dst_mask = 0;
	u_int16_t pad2 = 0;
}flow;

typedef struct export_flow_t
{
	flow_header_t nfthdr;
	flow payload;
}export_flow_t;

typedef std::tuple<std::string, std::string, u_int16_t, u_int16_t, int> flow_key_t;


/**
 * @brief Calculate time since the capture device booted in miliseconds
 * 
 * @param current Current time
 * @param boot Boot time of capture device
 * @return u_int32_t Time in miliseconds
*/
u_int32_t get_uptime(timeval current, timeval boot);


/// Function for debugging purposes
void print_flow(flow my_flow);

/**
 * @brief Find the oldest key of flow in map
 * 
 * @param my_map Map structure with flows
 * @return flow_key_t The key of the oldest flow in map
*/
flow_key_t find_oldest_flow_key(std::map<flow_key_t, flow> &my_map);


#endif