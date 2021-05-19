/**
 * @file  htab_bucket_count.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 *
 * Compiled : gcc version 9.3.0
 */

#include <stdio.h>

#include "htab.h"
#include "htab_t.h"

/**
 * Get the number of pointers in hash table
 * 
 * @param t Pointer to hash table structure
 * @return Number of pointers in array
 */
size_t htab_bucket_count(const htab_t * t){
    return t->arr_size;
}