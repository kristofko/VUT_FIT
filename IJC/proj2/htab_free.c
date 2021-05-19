/**
 * @file  htab_free.c
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
 * Function to destroy given hash table structure
 * 
 * @param t Pointer to hash table structure
 */

void htab_free(htab_t * t){
    htab_clear(t);
    free(t);

    return;
}
