/**
 * @file  htab_erase.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  7.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "htab.h"
#include "htab_t.h"

/**
 * Find item in hash table with given key,
 * erase it, if it exists, if not return false
 * 
 * @param[in] t Pointer to hash table structure
 * @param key String to find in table
 * @return true if key exists, false otherwise
 */
bool htab_erase(htab_t * t, htab_key_t key){
    size_t index = (htab_hash_function(key) % t->arr_size);

    htab_item *tmp = t->data[index];
    htab_item *prev = NULL;
    while (tmp != NULL){
        if (strcmp(tmp->pair.key, key) == 0 ){
            // In case the found item is first one in row 
            if (prev == NULL){
                prev = tmp->next; // Using prev as *next
                free((char *)tmp->pair.key);
                free(tmp);
                t->data[index] = prev;
                return true;
            }
            else {
                prev->next = tmp->next;
                free((char *)tmp->pair.key);
                free(tmp);
                return true;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }

    return false;
}