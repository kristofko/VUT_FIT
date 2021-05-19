/**
 * @file htab_lookup_add.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 *
 * Compiled : gcc version 9.3.0
 */


#include <stdio.h>
#include <stdlib.h>


#include "htab_t.h"


/**
 * Function to find a given word in hash table structure. 
 * If item doesnt exists, create a new one and add it in structure.
 *
 * @param[in] t Pointer to hash table structure.
 * @param[in] key Word to find hash table.
 * @return Pointer to htab_pair_t.
 */
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){

    htab_pair_t *tmp = htab_find(t, key);
    if (tmp != NULL){
        return tmp;
    }  
    else{
        size_t hash_index = ( htab_hash_function(key) % t->arr_size );
        htab_item *tmp_item = t->data[hash_index];
        htab_item *previous = NULL;
        // Find the last and last - 1 item 
        while (tmp_item != NULL){
            previous = tmp_item;
            tmp_item = tmp_item->next;
        }
        tmp_item = malloc (sizeof(htab_item));
        tmp_item->pair.key = malloc (sizeof(char) * strlen(key) + 1);

        if (previous != NULL){
        previous->next = tmp_item;
        }
        else {
            t->data[hash_index] = tmp_item;
        }
        tmp_item->pair.value = 0;
        
        // Cast const char to char to avoid warnings 
        strcpy( (char *)tmp_item->pair.key, key );
        tmp_item->next = NULL;

        t->size++;
        return &tmp_item->pair;
    }
}
