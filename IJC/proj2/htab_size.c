/**
 * @file  htab_size.c
 * Solution to homework IJC-DU2 b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>


#include "htab_t.h"

/**
 * Function to return number of items in hash table
 * @param t Pointer to hash_table
 * @return Returns number of items in hash table
 */
size_t htab_size(const htab_t * t){
    return t->size;
}