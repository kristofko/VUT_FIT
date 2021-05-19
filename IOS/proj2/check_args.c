#include <stdio.h>
#include <stdlib.h>
#include "proj2.h"

/** 
 * Function to check the argumnets of the program
 * @param argc Num of arguments
 * @param argv Program arguments
 * @return 0 if no problems occured, -1 otherwise
*/
int check_args(int argc, char **argv){
	if (argc != 5){
		fprintf(stderr, "Wrong number of arguments, aborting...");
		return -1;
	}
	char **temp = NULL;
	int Num_of_elves = strtol(argv[1], temp, 10);
	// 1st Argument	
	if (temp != NULL){
		return -1;
	}
	if (Num_of_elves <= 0 || Num_of_elves > MAX_ELF){
		fprintf(stderr, "Num of elfes must be in range of 0 to %d\n", MAX_ELF);
		return -1;
	}
	// 2nd Argument
	int Num_of_reindeer = strtol(argv[2], temp, 10);
	if (temp != NULL){
		return -1;
	}
	if (Num_of_reindeer <= 0 || Num_of_reindeer > MAX_REINDEER){
		fprintf(stderr, "Num of reindeers must be in range of 0 to %d\n", MAX_REINDEER);
		return -1;
	}
	// 3rd argument
	int elf_work = strtol (argv[3], temp, 10);
	if (temp != NULL){
		return -1;
	}
	if (elf_work < 0 || elf_work > MAX_WORK){
		fprintf(stderr, "Max working time of elves must be in range of 0 to %d\n", MAX_WORK);
		return -1;
	}
	// 4th argument
	int reindeer_work = strtol(argv[4], temp, 10);
	if (temp != NULL){
		fprintf(stderr,"Not a number argument occured. aborting\n");
		return -1;
	}
	if (reindeer_work < 0 || reindeer_work > MAX_WORK){
		fprintf(stderr, "Max workig time of reindeers must be in range of 0 to %d\n", MAX_WORK);
		return -1;
	}

	// No problems occured
	return 0;

}