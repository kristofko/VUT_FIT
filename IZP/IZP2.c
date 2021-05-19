/* @file sps.c
* 2nd IZP project
* File editing
* Author: Krištof Šiška
* Login: xsiska16
*
* Date: December 2020
                            */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

void drow();
void dcol();
void irow();

char** get_commands();
void get_table();
void get_selector();
int get_num_rows();
int get_num_cmd();
int get_num_cells();

void allocate_table();
void print_table();
void fill_table();
void process_table();

void free_selector();
void free_cmds();
void free_table();

typedef struct select_t{
    char *row;
    char *cell;
}select_t;

typedef struct table_t{
    int num_of_cells;
    char **table;
    int *cap;
}table_t;

#define BASE 10

int main(int argc, char **argv) {

    char *cmd_seq = argv[argc - 2];
    char **cmd =  get_commands((get_num_cmd(cmd_seq)), cmd_seq);
    char *file_name = argv[argc - 1];
    int num_rows;
    int num_cmd = get_num_cmd(cmd_seq);

    char *delims = " ";
    if (argc > 3 && (!strcmp(argv[1],"-d"))) {
        delims = argv[2];
    }
    num_rows = get_num_rows(file_name);
    table_t *t = malloc (sizeof(table_t) * num_rows);
    FILE *f = fopen(file_name, "r");

    int num_of_cells;
    char temp[500];
    // int i is index for rows
    int i = 0;
    // int j is index for cells
    int j = 0;
    while(fgets(temp, 500, f) != NULL) {
        num_of_cells = get_num_cells(temp,delims);
        t[i].table = malloc(sizeof(char *) * num_of_cells);
        t[i].cap = malloc(sizeof(int) * num_of_cells);
        t[i].num_of_cells = num_of_cells;

        for (j = 0; j < num_of_cells; j++) {
            t[i].table[j] = malloc(sizeof(char) * BASE);
            t[i].cap[j] = BASE;
        }
        i++;
    }
    fclose(f);

    select_t *selector = malloc(sizeof(select_t));
    selector->row = malloc (sizeof(char) * BASE);
    selector->cell = malloc (sizeof(char) * BASE);
    selector->row[0] = '1';
    selector->cell[0] = '1';

    fill_table(t,file_name,delims);
    process_table(t,selector, cmd, num_cmd);
    print_table(t, delims, num_rows, file_name);
    free_table(t,num_rows);
    free_cmds(cmd, num_cmd);
    free_selector(selector);

    return 0;
}

/*
Function to free allocated memory in cmd
*/

void free_cmds(char **cmd, int num_of_cmd) {
    for (int i = 0; i < num_of_cmd; i++) {
        free(cmd[i]);
    }
    return;
}

int get_num_cmd(char *cmd_seq){
    int count = 1;
    int len = strlen(cmd_seq);
    for (int i  = 0; i < len; i++){
        if (cmd_seq[i] == ';' || cmd_seq[i] == ' ') {
            count++;
        }
    }
    return count;
}

/*
Commnnd parser from command sequence in arguments
*/
char **get_commands(int num_of_cmd, char *cmd_seq){
    int k = 0;
    int j;
    int count;
    char *ptr;
    char **cmd = malloc (sizeof (char *) * num_of_cmd);
    int len = strlen(cmd_seq);

    for (int i = 0; i < num_of_cmd; i++){
        count = 0;
        j = 0;
        cmd[i] = malloc(sizeof(char) * BASE);
        for (k = k; k < len; k++){
            if (count == BASE){
                ptr = realloc(cmd[i], 2 * BASE);
                cmd[i] = ptr;
                count = 0;
            }
            if (cmd_seq[k] == ';' || cmd_seq[k] == ' '){
                k++;
                cmd[i][j] = '\0';
                break;
            }
            cmd[i][j] = cmd_seq[k];
            j++;
            count++;
        }
    }

    return cmd;
}

/*
For some reason, table is allocated right from code
from this function in main, but not right if this function is used
*/
void allocate_table(table_t *t, char *file_name, char *delims){
    FILE *f = fopen(file_name, "r");

    int num_of_cells;
    char temp[500];
    // int i is index for rows
    int i = 0;
    // int j is index for cells
    int j = 0;
    while(fgets(temp, 500, f) != NULL) {
        num_of_cells = get_num_cells(temp,delims);
        t[i].table = malloc(sizeof(char *) * num_of_cells);
        t[i].cap = malloc(sizeof(int) * num_of_cells);
        t[i].num_of_cells = num_of_cells;

        for (j = 0; j < num_of_cells; j++) {
            t[i].table[j] = malloc(sizeof(char) * BASE);
            t[i].table[j] = NULL;
            t[i].cap[j] = BASE;
        }
        i++;
    }
    return;
}
void enlarge_cell(table_t *t, int n_line, int n_cell){
    char *ptr = realloc (t[n_line].table[n_cell], sizeof(char) * t[n_line].cap[n_cell] * 2);
    if (ptr == NULL){
        fprintf(stderr, "Realloc problem");
    }
    else {
        t[n_line].table[n_cell] = ptr;
        t[n_line].cap[n_cell] = t[n_line].cap[n_cell] * 2;
    }
}

void fill_table(table_t *t, char *file_name, char *delims){
    FILE *f = fopen(file_name, "r");
    char temp[500];
    char temp_cell[500];
    int n_line = -1;
    int n_cell;
    int i;
    int j;
    int len;
    int len_del = strlen (delims);
    bool fill = true;
    while (fgets(temp, 500, f) != NULL){
        len = strlen(temp);
        n_line++;
        j = 0;
        n_cell = 0;
        for (i = 0; i < len; i++){
            fill = true;
            for (int k = 0; k < len_del; k++){
                if (temp[i] == delims[k] || i == len - 1){
                    temp_cell[j] = '\0';
                    if ( (int) strlen(temp_cell) >  t[n_line].cap[n_cell]){
                        enlarge_cell(t, n_line, n_cell);
                    }

                    memcpy(t[n_line].table[n_cell],temp_cell, strlen(temp_cell));

                    n_cell++;
                    j = 0;
                    fill = false;
                    break;
                }
            }
            if (fill){
                temp_cell[j] = temp[i];
                j++;
            }
        }
    }
}

void print_table(table_t *t, char *delims, int num_rows, char *file_name) {
    FILE *f = fopen(file_name, "w");
    int row_check = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < t[i].num_of_cells; j++) {
            fprintf(f,"%s", t[i].table[j]);
            if (t[i].table[j][0] == '\0') {
                row_check++;
            }
            if (j != t[i].num_of_cells - 1 && t[i].table[j][0] != '\0') {
                fprintf(f,"%c", delims[0]);

            }
        }
        if (t[i].num_of_cells != row_check) {
            fprintf(f,"\n");
        }
        row_check = 0;

    }
    fclose(f);
    return;
}

void free_table(table_t *t, int num_rows) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < t[i].num_of_cells; j++){
            free(t[i].table[j]);
        }
        free(t[i].cap);
    }
    free(t);

    return;
}
int get_num_rows(char *file_name){
    FILE *f = fopen(file_name, "r");
    int count = 0;
    char temp[500];
    while (fgets(temp, 500, f) != NULL){
        count++;
    }
    rewind(f);
    fclose(f);
    return count;

}
int get_num_cells(char *temp, char *delims){
    int count = 1;
    int len = strlen(temp);
    int len_delim = strlen(delims);
    for (int i = 0; i < len; i++){
        if (temp[i] != 92 || temp[i - 1] != 92){
            for (int j = 0; j < len_delim; j++){
                if (temp[i] == delims[j]){
                    count++;
                    break;
                }
            }
        }
    }
    return count;
}
void process_table(table_t *t,select_t *selector, char *cmd[],int num_cmd){
    for (int i = 0; i < num_cmd; i++){
        if (cmd[i][0] == '['){
            get_selector(selector, cmd[i]);
        }
        if (!strcmp(cmd[i], "dcol")){
            dcol(t, selector);
        }
        if (!strcmp(cmd[i], "drow")){
            drow(t,selector);
        }
         /*TODO*/
    }
    return;
}

/*Function to get the values for selected row and col*/
/*command is a argument which starts with '[' */
void get_selector(select_t *selector, char* command) {
    // getting rid of any value that selector had previously been set on
    free(selector->row);
    free(selector->cell);
    selector->row = malloc (sizeof(char) * BASE);
    selector->cell = malloc (sizeof(char) * BASE);
    int len = strlen (command);
    char n_row[5];
    int j = 0;
    char n_cell[5];
    int k = 0;
    bool change = false;
    // i = i and i < len-1 because on pos 0 and last are '[' and ']'
    for (int i = 1; i < len - 1; i++){
        if (command[i] == ',' && change == false){
            change = true;
            i++;
        }
        if (change == false){
            n_row[j] = command[i];
            j++;
        }
        else {
            n_cell[k] = command[i];
            k++;
        }
    }
    n_row[j] = '\0';
    n_cell[k] = '\0';
    if (n_row[0] == '_'){
        selector->row[0] = '_';
    }
    else if (n_cell[0] == '_'){
        selector->cell[0] = '_';
    }
    else if (!strcmp(n_row,"max")){
        // TODO
    }
    else if (!strcmp(n_row, "min")){
        // TODO
    }
    else {
        memmove(selector->row, n_row, strlen(n_row));
        memmove(selector->cell, n_cell, strlen(n_cell));
    }
    return;
}
/* Inserts row after the selected row
 * This function is not complete and will
 * execute even if given argument is irow */
//TODO
void irow(table_t *t, select_t *selector, int *num_rows){
    int n_row = *num_rows;
    table_t *ptr = realloc (t, sizeof(table_t) *(n_row + 1));
    int r = atoi(selector->row);
    int i;
    for (i = n_row; i >= r; i--){
        ptr[i] = ptr[i - 1];
    }
    t[r].table = malloc (sizeof (char *) * t[i+1].num_of_cells);
    for (int j = 0; j < ptr[i + 1].num_of_cells; j++){
        ptr[r].table[j] = malloc (sizeof(char) * BASE);
        ptr[r].cap[j] = BASE;
    }
    ptr[r].num_of_cells = ptr[i + 1].num_of_cells;
    t = ptr;
    return;
}

/*Deletes the selected column*/
void dcol(table_t *t, select_t *selector){
    int r = atoi(selector->row) - 1;
    int c = atoi(selector->cell) - 1;
    t[r].table[c][0] = '\0';
    return;
}
/*Deletes the selected row*/
void drow(table_t *t, select_t *selector) {
    int r = atoi (selector->row) - 1;
    for (int i = 0; i < t[r].num_of_cells; i++){
        t[r].table[i][0] = '\0';
    }
    return;
}

/*Function to free the memory allocated in selector*/
void free_selector(select_t *selector){
    free(selector->row);
    free(selector->cell);
    free(selector);
    return;
}