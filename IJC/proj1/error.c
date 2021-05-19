/*  
 * File : error.c
 * Solution to homework IJC-DU1 a) b)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 9.3.2021 
 * 
 * Compiled : gcc version 9.3.0 
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void warning_msg(const char *fmt, ...){
    va_list list;
    va_start (list, fmt);
    fprintf(stderr, "Chyba: ");
    vfprintf(stderr, fmt, list);
    va_end(list);

    return;
}

void error_exit(const char *fmt, ...){
    va_list list;
    va_start(list, fmt);
    fprintf(stderr, "Chyba: ");
    vfprintf(stderr, fmt, list);
    va_end(list);
    exit(1);
}