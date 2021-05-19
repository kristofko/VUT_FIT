/**
 * @file  htab_clear.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  7.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include <stdlib.h>


#include "htab_t.h"

/**
 * Function to delete every item from hash table.
 * Hash table is left initialized, but itemless
 * 
 * @param t Pointer to hash table structure
 */
void htab_clear(htab_t * t){
    size_t end = t->arr_size;
    htab_item *current = NULL;
    htab_item *next_item = NULL;
    for (size_t i = 0; i < end; i++ ){
        current = t->data[i];
        while (current != NULL){
            next_item = current->next;
            // First free the key, then item
            free( (char *)current->pair.key);
            free(current);
            current = next_item;
        }
    // Set items to NULL, but dont free hash table
	t->data[i] = NULL;
    }

    return;
}
