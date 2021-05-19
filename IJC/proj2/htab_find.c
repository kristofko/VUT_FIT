/**
 * @file  htab_find.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 *
 * Compiled : gcc version 9.3.0
 */


#include <stdio.h>
#include "htab_t.h"

/**
 * Finds htab_pair in hash table,
 * if it doesnt exits, returns NULL
 * 
 * @param t Pointer to hash table structure
 * @param key Word to find in hash table 
 * @return Pointer to pair or NULL
 */
htab_pair_t * htab_find(htab_t * t, htab_key_t key){
    size_t index = ( htab_hash_function(key) % t->arr_size );
    htab_item *tmp = t->data[index];
    while (tmp != NULL){
        if (!strcmp(tmp->pair.key , key)){
            return &tmp->pair;
        }
        tmp = tmp->next;
    }
    return NULL;
}
