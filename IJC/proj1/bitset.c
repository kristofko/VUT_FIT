/*  
 * File : bitset.c
 * Solution to homework IJC-DU1 a) b)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 24.3.2021 
 * 
 * Compiled : gcc version 9.3.0 
 */

#include "bitset.h"

extern inline int  bitset_getbit(bitset_t jmeno_pole, unsigned long index);
extern inline void bitset_setbit(bitset_t jmeno_pole, unsigned long index, unsigned long vyraz);
extern inline unsigned long bitset_size(bitset_t jmeno_pole);
extern inline void bitset_free(bitset_t jmeno_pole);