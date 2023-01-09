/**
 * @file arg_parser.cpp
 * @author  KriĹĄtof Ĺ iĹĄka - xsiska16, FIT
 * 
 * Last update : Nov 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"

bool check_args(int argc, char **argv, args_t *arguments){
	// Default values 
	arguments->filename = NULL;
	arguments->active = DEFAULTACTIVE;
	arguments->inactive = DEFAULTINACTIVE;
	arguments->count = DEFAULTCOUNT;


	for (int i = 0; i < argc; i++){
		if (!strcmp(argv[i], FILENAMEFLAG)){
			if (i+1 < argc){
				arguments->filename = argv[i+1];
			}
		}
		else if(!strcmp(argv[i], COLLECOTRFLAG)){
			if (i+1 < argc){
				strcpy(arguments->collector, argv[i+1]);
			}
		}
		else if (!strcmp(argv[i], ACTIVEFLAG)){
			if (i+1 < argc){
				arguments->active = atoi(argv[i+1]);
			}
		}
		else if (!strcmp(argv[i], INACTIVEFLAG)){
			if (i+1 < argc){
				arguments->inactive = atoi(argv[i+1]);
			}
		}
		else if (!strcmp(argv[i], COUNTFLAG)){
			if (i+1 < argc){
				arguments->count = atoi(argv[i+1]);
			}
		}
	}

	return true;
}

void print_args(args_t *arguments){
	printf("%s %s %d %d %d\n", arguments->filename, arguments->collector, arguments->active, arguments->inactive, arguments->count);
}