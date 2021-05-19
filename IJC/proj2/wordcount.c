/**
 * Solution to homework IJC-DU2  b)
 * @file : wordcount.c
 * @author : Krištof Šiška - xsiska16, FIT
 * @date : 3.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include "htab.h"
#include "io.h"

void print_pair(htab_pair_t *data);

#define MAX_LEN 127
#define EXIT_ERROR -1
#define EXIT_SUCCESS 0

int main(int argc, char **argv) {
    if (argc != 2){
        fprintf(stderr, "Wrong number of arguments, aborting...\n");
        return EXIT_ERROR;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL){
        fprintf(stderr, "Couldnt open %s, aborting...\n", argv[1]);
        return EXIT_ERROR;
    }
    htab_t *table = htab_init(15000);
    if (table == NULL){
        fprintf(stderr, "Problem with malloc, aborting...\n");
        return EXIT_ERROR;
    }

    // Variable to keep track of error printings
    bool print_err = true;
    char word[MAX_LEN + 1];
    int length = 0;
    htab_pair_t *curr_pair;
    while  ( (length = read_word(word, MAX_LEN, f)) != EOF){
        if (length > MAX_LEN && print_err){
            fprintf (stderr, "Word is longer than %d characters\n", MAX_LEN);
            print_err = false;
        }
        curr_pair = htab_lookup_add(table, word);
        curr_pair->value++;
    }
    // Print content of table     
    htab_for_each(table, &print_pair);
    
    // Test function 
    #ifdef MOVETEST
        htab_t *new_map = htab_move(3000, table);
        htab_for_each(new_map, &print_pair);
        htab_free(new_map);
    #endif

    htab_free(table);
    fclose(f);
    
    return EXIT_SUCCESS;

}

void print_pair(htab_pair_t *data){
    printf("%s  : %d\n", data->key, data->value);
}