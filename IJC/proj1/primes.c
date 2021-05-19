/*  
 * File : primes.c
 * Solution to homework IJC-DU1 a)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 5.3.2021 
 * 
 * Compiled : gcc version 9.3.0 
 */


#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "eratosthenes.h"
#include "bitset.h"

#define PRINT_NUM 10
#define ERASTO_END 200000000

int main() {
     clock_t start = clock();
     
     bitset_create(Arr, ERASTO_END);
     eratosthenes(Arr);
     
     int last_primes[10];
     int count = 0;

     for (int i = ERASTO_END - 1; count < PRINT_NUM; i--){
          if (bitset_getbit(Arr, i) == 0){
               last_primes[count] = i;
               count++;
          }
     }
     for (int i = 9; i >= 0; i--){
          printf("%d\n", last_primes[i]);
     }
     fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
     return 0;    
}