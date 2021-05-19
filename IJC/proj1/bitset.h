/**  
 * File : bitset.h
 * Solution to homework IJC-DU1 a) b)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 5.3.2021 
 * 
 * Compiled : gcc version 9.3.0 
 */
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

typedef unsigned long *bitset_t;

typedef unsigned long bitset_index_t;

// Amount of bits in unsigned long
#define SIZE (sizeof(unsigned long) * CHAR_BIT)

/*  Creates an array with velikost bits + (1/2)(sizeof(unsigned long)) , name jmeno_pole
 * The first extra unsigned long is for storing the size of bit_array, the second one, 
 * which may or may not be included, is for the last few bits
 */
#define bitset_create(jmeno_pole,velikost) _Static_assert (velikost > 0, "Size must be greater than 0"); \
        unsigned long jmeno_pole[((velikost/(SIZE))+(velikost % (SIZE) > 0 ? 1 : 0)+1)] = { velikost, 0 }


#define bitset_alloc(jmeno_pole,velikost) \
    bitset_t jmeno_pole = calloc(((velikost/SIZE)+((velikost % SIZE) > 0 ? 1 : 0)+1),sizeof(unsigned long)); \
    jmeno_pole[0] = velikost

#ifdef USE_INLINE
     inline void bitset_free(bitset_t jmeno_pole){
        free(jmeno_pole);
        return;
    }
#else
    #define bitset_free(jmeno_pole) free(jmeno_pole)

#endif

#ifdef USE_INLINE
     inline unsigned long bitset_size(bitset_t jmeno_pole){
        return jmeno_pole[0];
    }
#else
    #define bitset_size(jmeno_pole)  jmeno_pole[0]

#endif

#ifdef USE_INLINE 
     inline void bitset_setbit(bitset_t jmeno_pole, unsigned long index, unsigned long vyraz) {
        if (index > bitset_size(jmeno_pole)){
            warning_msg("%lu je neplatny index", index);
        }
        if (vyraz){
            jmeno_pole[(index/SIZE) + 1] |= (1UL << (index % SIZE));
        }
        else {
            jmeno_pole[(index/SIZE) + 1]  &= ~((1UL << (index % SIZE)));
        }
    }
#else 
    // Set bit to 1 or 0 
    #define bitset_setbit(jmeno_pole,index,vyraz) \
                    do \
                    { \
                        if (index > bitset_size(jmeno_pole)){ \
                            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(jmeno_pole)); \
                        } \
                        if (vyraz)  \
                        {  \
                        jmeno_pole[(index/SIZE) + 1] |= (1UL << (index % SIZE)); \
                        } \
                        else \
                        { \
                        jmeno_pole[(index/SIZE) + 1]  &= ~((1UL << (index % SIZE))); \
                        } \
                    } \
                    while(0)

#endif

#ifdef USE_INLINE
     inline int bitset_getbit(bitset_t jmeno_pole, unsigned long index) {
        if (index > bitset_size(jmeno_pole)){
            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(jmeno_pole));
            return EXIT_FAILURE;
        }
        else {
            return (unsigned long) ((jmeno_pole[(index/SIZE) + 1] & (1UL << (index % (SIZE)))) > 0);
        }
    } 
#else 
    // Gets the bit at given index
    #define bitset_getbit(jmeno_pole,index)  \
        /* Using comma operator to get around problems with ternary operator */ \
        ((unsigned long)index > bitset_size(jmeno_pole) ? error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(jmeno_pole)) :(void) 0),  \
        ((jmeno_pole[(index/SIZE) + 1] & (1UL << (index % SIZE))) > 0) 
#endif

/* Get the Lsb of the char in array at position index */
#define ppm_getbit(jmeno_pole,index) ((jmeno_pole[index] & (1UL << 0 )) > 0)

/* Sets the Given bit at position index to 0 or 1 */
#define ppm_setbit(jmeno, index, vyraz) do \
                    { \
                        if (vyraz)  \
                        {  \
                        jmeno |= (1UL << index); \
                        } \
                        else \
                        { \
                        jmeno &= ~(1UL << index); \
                        } \
                    } \
                    while(0)
                    