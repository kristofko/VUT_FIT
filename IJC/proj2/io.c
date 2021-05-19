/**
 *  Solution to homework IJC-DU2  b)
 * Implementation of read_word function,
 * used in wordcount.c
 *  
 * @file  io.c
 * @author  Krištof Šiška - xsiska16, FIT
 * @date  6.4.2021
 * 
 * Compiled : gcc version 9.3.0
 */
#include <stdio.h>
#include <ctype.h>

#include "io.h"

/**
 * Function to read a word from file.
 * Word is a sequence of characters, separated by whitespaces
 * @param s Variable to load the word into
 * @param max Max number of characters in word
 * @param f File pointer
 * @return Return the number of characters in word
 */ 
int read_word(char *s, int max, FILE *f){
    int c;
    int i = 0;
    // First get rid of all whitespaces between words
    while ( (c = fgetc(f)) != EOF ){
        if (!isspace(c)){
            // After first not white-space character is found
            ungetc(c,f);
            break;
        }
    }
    while ( ((c = fgetc(f)) != EOF) ){
        if (isspace(c) || c == EOF){
            break;
        }
        if (i >= max - 1){
            s[i] = '\0';
            while ( (c = fgetc(f)) != EOF ){
                if (isspace(c)){
                ungetc(c,f);
                break;
                }   
            }
            return max + 1;
        }
        s[i] = c;
        i++;
    }
    s[i] = '\0';
    if (c == EOF && s[0] == '\0'){
        return EOF;
    }
    
    return i;

}
