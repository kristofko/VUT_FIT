/**
 * @file  htab_for_each.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  9.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include "htab_t.h"
/** 
 * Call function f on every item in hash table
 * 
 * @param t Pointer to hast table structure
 * @param f Pointer to function to be called on every htab_pair_t
*/
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){
    htab_item *current = NULL;
    int end = t->arr_size;
    for (int i = 0 ; i < end; i++){
        current = t->data[i];
        while (current != NULL){
            f(&current->pair);
            current = current->next;
        }
    }

    return;
}