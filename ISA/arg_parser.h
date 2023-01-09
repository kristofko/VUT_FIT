/**
 * @file arg_parser.h
 * @author  Krištof Šiška - xsiska16, FIT
 * 
 * Last update : Nov 2022
 *
 */


#ifndef ARG_PARSER
#define ARG_PARSER

#include <stdbool.h>


#define FILENAMEFLAG "-f"
#define COLLECOTRFLAG "-c"
#define ACTIVEFLAG "-a"
#define INACTIVEFLAG "-i"
#define COUNTFLAG "-m"

#define DEFAULTACTIVE 60
#define DEFAULTINACTIVE 10
#define DEFAULTCOUNT 1024


typedef struct args_t
{
	char *filename = nullptr;
	char collector[256] = "127.0.0.1:2055";
	int active;
	int inactive;
	int count;
} args_t;

/**
 * @brief Function to check and parse parameters
  */
bool check_args(int argc, char **argv, args_t *arguments);

/**
 * @brief Control function used for debugging purposes  
  */ 

void print_args(args_t *arguments);

#endif