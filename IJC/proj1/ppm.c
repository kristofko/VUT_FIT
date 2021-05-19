/*  
 * File : ppm.c
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 5.3.2021
 * 
 * Compiled : gcc version 9.3.0 
 */
#include <assert.h>     
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "ppm.h"

/* Function to read data from ppm file and store
 * it in a struct, along with xsize and ysize
 */

const unsigned long limit = 8000 * 8000 * 3;

struct ppm *ppm_read(const char *filename){
    FILE *f = fopen(filename, "r");
    
    if (!f){
        error_exit("Cannot open %s file", filename);
    }
    
    char Magic[3];
    int xsize, ysize, max_rgb;
    fscanf(f, "%s %d %d %d ", Magic, &xsize, &ysize, &max_rgb);

    if (strcmp("P6", Magic) != 0){
        warning_msg("ppm File doesnt start with P6");
        fclose(f);
        return NULL;
    }
    
    if (max_rgb > 255){
        warning_msg("%d max_rgb is higher than 255", max_rgb);
        fclose(f);
        return NULL;
    }
    
    unsigned long size = 3 * xsize * ysize;
    assert(size <= limit);
    if (size > limit){
        fclose(f);
        error_exit("Size of ppm file is over the limit. Size:  %lu ", size);
    }
    struct ppm *Image = malloc (size + sizeof(struct ppm));
    
    if(Image == NULL){
        fclose(f);
        warning_msg("Problem with Image malloc");
        return NULL;
    }

    Image->xsize = xsize;
    Image->ysize = ysize;
    for (unsigned long i = 0; i < size; i++){
        Image->data[i] = fgetc(f);
    }
    fclose(f);
    return Image;
}

void ppm_free(struct ppm *p ){
   free(p);
}