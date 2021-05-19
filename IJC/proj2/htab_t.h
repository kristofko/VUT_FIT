#ifndef __HTAB_T_H__
#define __HTAB_T_H__
/**
 * @file  htab_t.h
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 *
 * Compiled : gcc version 9.3.0
 */


#include "htab.h"

#include <stdio.h>

typedef struct htab_item {
    htab_pair_t pair;
    struct htab_item *next;
}htab_item;

struct htab { 
   size_t size;    //  Number of entries  
   size_t arr_size;  // Size of next array of pointers
   htab_item *data[]; //  Hash table data 
};

#endif // __HTAB_T_H__