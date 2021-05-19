/**
 * Program to print the last x lines of given file. 
 * 
 * @file  tail.c
 * Solution to homework IJC-DU2  a)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  29.3.2021
 * 
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

char **init_components(int n);

void free_components(char **shuffling_lines, int n);

int main(int argc, char **argv){
    int num_of_lines = 10;
    int usage = 1;
    char *lines_arg = NULL;
    char *filename = NULL;
    FILE *f;
    if (argc == 2){
        filename = argv[1];
    }
    if (argc == 3){
        if ( strcmp(argv[1], "-n" ) != 0){
            fprintf(stderr, "Wrong program arguments, missing -n\n");
            return -1;
        }
        lines_arg = argv[2];
        if (lines_arg[0] == '+') {
            usage = 2;
            }
        num_of_lines = atoi(lines_arg);
    }
    if (argc == 4){
        filename = argv[3];
        lines_arg = argv[2];
        if (lines_arg[0] == '+') {
            usage = 2;
            }
        num_of_lines = atoi(lines_arg);
    }
    
    if (argc == 3 || argc == 1){
        f = stdin;
    }
    else {
        f = fopen (filename, "r");
        if (f == NULL){
            fprintf(stderr, "Unable to open file %s, file may not exist\n", filename);
            return -1;
        }
    }

    char **shuffling_lines = NULL;
    if (usage == 1){
        shuffling_lines = init_components(num_of_lines);
    }
    
    int i = 0;
    int j = 0;
    /* If line is longer than MAX_LINE, the end is ignored */
    char line[MAX_LINE + 1];
    while ( fgets(line, MAX_LINE, f) != NULL ) {

        if (usage == 1){
           j = i % num_of_lines;
            memcpy(shuffling_lines[j], line, MAX_LINE);
           /* USAGE WITHOUT + .... Last n line */ 
        }

        i++;
        if (usage == 2){
            if (i >= num_of_lines){
                printf("%s", line);
            }
            /* USAGE WITH +... Lines > n */
        } 
    }
    
    if (usage == 1){
        j = 0;
        for (; j < num_of_lines; i++){
            printf("%s", shuffling_lines[i % num_of_lines]);
            j++;
        }
        free_components(shuffling_lines, num_of_lines);
    }
    printf("\n");

    return 0;
}

/** 
 * Components constructor
*/
char **init_components(int n){
    char **shuffling_lines = malloc(n * sizeof (char *));
    if (shuffling_lines == NULL){
        fprintf(stderr, "Malloc problem\n");
        return NULL;
    }
    for (int i = 0; i < n; i++){
        shuffling_lines[i] = malloc( MAX_LINE * sizeof(char) );
        if (shuffling_lines[i] == NULL){
        fprintf(stderr, "Malloc problem\n");
        return NULL;
        } 
    }

    return shuffling_lines;
}

/** 
 * Components destructor
*/
void free_components(char **shuffling_lines, int n){
    for (int i = 0; i < n; i++){
        free(shuffling_lines[i]);
    }

    free(shuffling_lines);

    return;
}