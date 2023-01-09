/**
 * @file exporter.h
 * @author Kristof Siska, xsiska16
 * 
 * Header file for function definitons for netflow exporting
*/


#ifndef EXPORTER_H
#define EXPORTER_H

#include "flow.h"
#include "arg_parser.h"



/**
 * @brief Export flows with regards to timers or cache size
  */
void export_flows(std::map<flow_key_t, flow>&my_map, timeval current_time, args_t args, timeval boot);

/**
 * @brief Get the port number from -c parameter
 * 
 * @return Port number if port is given, 2055 otherwise
  */
int get_port(args_t args);

/**
 * @brief Get the string representation of given -c param without port
  */
void get_host_or_ip(args_t args, char *host_name);

/**
 * @brief Send the given flow via already connected socket
  */
void send_flow(int sock, export_flow_t export_flow);

/**
 * @brief Export all remaining netflows in map. Function is used at the end of reading pcap file
  */
void export_all_netflows(std::map<flow_key_t, flow>&my_map, timeval current_time, timeval boot_time, args_t args);

/**
 * @brief Returns the socket descriptor which will be used to send data
  */
int get_connected_sock(args_t args);

/**
 * @brief Get the export_flow_t header structure to network byte order
  */
export_flow_t htons_header(export_flow_t exp_flow);

/**
 * @brief Get the export_flow_t payload structure to network byte order
  */
export_flow_t htons_payload(export_flow_t exp_flow);

/**
 * @brief Find and export the oldest flow in map
  */
void send_oldest_flow();

#endif