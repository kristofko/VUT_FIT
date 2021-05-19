/*  
 * File : erastothenes.c
 * Solution to homework IJC-DU1 b)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 5.3.2021
 *  
 * Compiled : gcc version 9.3.0 
 */

#include <stdio.h>
#include <ctype.h>
#include "bitset.h"
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"

int main (int argc, char **argv) {
    if (argc != 2 ){
        error_exit("Wrong number of arguments (ppm file may or may not be given)");
    }
    char *filename = argv[1];
    
    struct ppm *Image = ppm_read(filename);
    if (Image == NULL){
        error_exit("%s file cannot be accessed", filename);
    }
    /* Variable to store bits */
    char c = 0;
   
    int written_bits = 0;
    
    int size = Image->xsize * Image->ysize * 3;
    assert(size > 0);
    bitset_alloc(Arr, size);
    if (Arr == NULL){
        error_exit("problem with data allocation");
    }
    eratosthenes(Arr);

    for (int i = 23; i < size; i++){
        
        if (bitset_getbit(Arr, i) == 0){
           
            int bit = ppm_getbit(Image->data, i);
            ppm_setbit(c, written_bits, bit);
            written_bits++;
        }
        if (written_bits == CHAR_BIT){
            if (c == '\0'){
                break;
                // End of decoding
            }
            fprintf(stdout, "%c", c);
            written_bits = 0;
            // Prints the current character and starts new cycle
        }
    }
    /* Reading must end on terminating null */
    if (c != '\0'){
        error_exit("given file doesnt end with terminating null");
    }
    else {
        fprintf(stdout, "%c", c);
    }
     // Odstranit pri odovzdavani - pouzite na viditelnost
    printf("\n");
    ppm_free(Image);

    return 0; 
}