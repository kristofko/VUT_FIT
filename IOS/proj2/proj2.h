/** 
 * Solution to IOS proj2 homework
 * 
 * Using semaphores, shared memory and forks 
 * to solve the Santa problem
 * 
 * @file proj2.h 
 * @author Krištof Šiška (xsiska16)
 * @date 16.4.2021
 */


#ifndef PROJ2
#define PROJ2

#define MAX_ELF 1000
#define MAX_REINDEER 20
#define MAX_WORK 1000

int check_args(int argc, char **argv);

int init_components();

void destruct_components();

void process_santa();

void process_elf();

void process_reindeer();

void check_sign();

void need_help();

void get_help();

void helping_elves();

void christmas_started();
#endif // proj2.h