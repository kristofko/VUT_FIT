/**
 * @file htab_hash_function.c
 * Solution to homework IJC-DU2  b)
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 *
 * Compiled : gcc version 9.3.0
 */


#include <stdio.h>
#include <stdint.h>

#include "htab_t.h"


#ifdef HASHTEST
// djb2 hash function taken from http://www.cse.yorku.ca/~oz/hash.html
    size_t htab_hash_function(htab_key_t str){
    uint32_t h=5381;     
          const unsigned char *p;
          for(p=(const unsigned char*)str; *p!='\0'; p++)
             h = ((h << 5) + h) + *p;   
          return h;
}

#else 
/** 
 * Basic hash function, used to generate hash of string.
 * 
 * @param str String to make hash from.
 * @return Hash of string.
 */
size_t htab_hash_function(htab_key_t str){
    uint32_t h=0;     // musí mít 32 bitů
          const unsigned char *p;
          for(p=(const unsigned char*)str; *p!='\0'; p++)
              h = 65599*h + *p;
          return h;
}
    
#endif
