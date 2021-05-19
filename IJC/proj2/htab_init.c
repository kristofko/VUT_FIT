/**
 * @file  htab_init.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  5.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "htab_t.h"

/**
 * Function to initialize hash table structure
 * 
 * @param[in] n Number of pointers in hash table
 * @return Pointer to hash table or NULL
 */ 
htab_t *htab_init(size_t n){
    htab_t *table = NULL;
    table = malloc(sizeof(htab_t) + sizeof(htab_pair_t *) * n);
    if (table == NULL){
        /* Print malloc error */
        fprintf(stderr, "Malloc problem, aborting program\n");
        exit(0);
        // TODO
    }
    for (unsigned int i = 0; i < n; i++){
        table->data[i] = NULL;
    }
    table->size = 0;
    table->arr_size = n;
    
    return table;
}