/*  
 * File : erastothenes.c
 * Solution to homework IJC-DU1 a)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 5.3.2021
 *  
 * Compiled : gcc version 9.3.0 
 */


#include <stdio.h>
#include <math.h>

#include "eratosthenes.h"
#include "bitset.h"

#define ERASTO_END 200000000

void eratosthenes(bitset_t Arr){
    unsigned long n = bitset_size(Arr);
    unsigned long end = sqrt(ERASTO_END);
    
    for (unsigned long i = 2; i < end; i++){
        
        if (bitset_getbit(Arr,i) == 0) {
            
            for (unsigned long j = 2 * i ; j < n; j+= i){
                
                bitset_setbit(Arr, j , 1);
            }
        }
    }
    
    return;
} 
            
    
    


