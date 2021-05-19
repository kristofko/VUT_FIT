 /*  
 * File : ppm.h
 * Solution to homework IJC-DU1 b)
 * Author : Krištof Šiška - xsiska16, FIT
 * Date : 5.3.2021 
 * 
 * Compiled : gcc version 9.3.0 
 */

 
 struct ppm {
        unsigned xsize;
        unsigned ysize;
        char data[];    // RGB bajty, celkem 3*xsize*ysize
     };
struct ppm *ppm_read(const char * filename);

void ppm_free(struct ppm *p );
 