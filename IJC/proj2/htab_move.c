
/**
 * @file  htab_move.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  7.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include <stdlib.h>


#include "htab_t.h"


/** Moves all data fron one table to another.
 *  First table remains initialized but itemless
 * 
 * @param n Number of pointers (arr_size) in new table
 * @param from Pointer to hash table to take data from
 * @return Returns pointer to a new table
 */
htab_t *htab_move(size_t n, htab_t *from){

    htab_t *table = htab_init(n);
    table->size = from->size;
    table->arr_size = n;

    htab_item *current = NULL;
    htab_pair_t *curr_pair = NULL;
    for (int i = 0; i < from->arr_size; i++){
        // Move data from one to another 
        current = from->data[i];
        // If there is no data in 1st pos of given row
        while (current != NULL){
            curr_pair = htab_lookup_add(table, current->pair.key); 
            curr_pair->value = current->pair.value;
            current = current->next;
        }
    }

    htab_clear(from); 

    return table;
}
