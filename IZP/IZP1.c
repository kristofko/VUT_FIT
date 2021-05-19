/* @file sheet.c
* 1st IZP project
* Editing file
* Author: Krištof Šiška
* Login: xsiska16
*
* Date: November 2020 
                            */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool error0(const char *check_2[], int argc, char *argv[]);

bool check_args(const char *check[], int argc, char *argv[]);

bool error1(int argc, int j, char* argv[]);

bool error2(int argc, int j, char* argv[], int i);

bool is_ntrl_num(char* to_check);

bool is_last();

bool is_greater(int num);

bool check_rows(char *argv[], int j);

void to_lower(int C,char* current_line, char* delims, int* ptr);

void to_upper(int C,char* current_line, char* delims, int* ptr);

void print_cols(int total_col, char *delims);

int num_of_cols(char *current_line, char *delims);

int rows(int count_row, char *N, char *M, bool last);

int beginswitch(char* current_line, int C, char* search, char *delims);

int contains(char* current_line, int C, char* search, char* delims);

void swap(int N, int M,  int *ptr, char *current_line, char *delims);

void print_base(char *base);

void print_temp(char* tmp, char* current_line, char *delims);

void change_del(char *delims, char *current_line);

void to_int(int C, char* current_line, char* delims, int* flag_print);

void roundd(int C, char* current_line, char* delims, int *flag_print);

void dcol(int argv_C, int count_col,char current_char, int* ptr_flag);

int drow(int arg_line, int count_line);

int drows(int arg_N, int arg_M, int count_line);

void dcols(int arg_N, int arg_M, int count_col,char current_char, int *ptr);

void icol(int arg_C, int count_col, char* delims, int* ptr);

void irow(int arg_C, int count_line, int total_col, char* delims);

int arow(char *arg_line);

void cset(int arg_col, char * delims, char *argv,char *current_line, int *ptr);

void copy(int N, int M, char *current_line, char *delims, int *ptr);

void move(int N, int M, char* current_line, char* delims, int* ptr);



   
#define NUM_OF_FC 17
#define MAX 10242 
#define MAX_COL 100
#define DEL 2


int main(int argc, char* argv[]) {
    const char* check_1[] = { "irow", "drow", "icol", "dcol", "tolower", "toupper", "round","int",
        "drows", "dcols", "copy", "swap", "move", "rows", "cset", "beginswitch", "contains" };
    const char* check_2[] = { "arow", "acol", '\0' };
    char* delims;
    int i;
    int selector = 1;
    int line_length;
    int count_col_char = 0;
    int count_col;
    int total_col;
    char current_line[MAX];
    int count_line = 0;
    int flag_arow = 0;
    int flag_print = 1;
    int flag_acol = 0;
    i = 0;
    int j;
    int flag_cset = 1;
    bool last;
    if (argc > 2) {
        if (strcmp(argv[1], "-d") == 0) {
            delims = argv[2];
        }
    }
    else {
        delims = " ";
    }
    if (argc > 1){
        if (error0(check_2, argc, argv)) {
            if (check_args(check_1, argc, argv) == false) {
                return 1;
            }
        }
    } 
    
   
    // Nacitanie riadku  textoveho suboru

    while (fgets(current_line, MAX, stdin) != NULL) {
        if (strlen(current_line) > MAX) {
            fprintf(stderr, "Problem s nacitanim riadku: prekrocena velkost");
        }
        if (!strcmp(argv[1], "-d")) {
            change_del(delims, current_line);
        }
        last = is_last();
        total_col = num_of_cols(current_line, delims);
        flag_print = 1;
        flag_cset = 1;
        count_line++;
        selector = 1;
        count_col = 1;
        flag_arow = 0;
        line_length = strlen(current_line);
        for (j = 0; j < argc; j++) {
            if (strcmp(argv[j], "rows") == 0) {
                selector = rows(count_line, argv[j + 1], argv[j + 2], last);
                break;
            }
            if (!strcmp(argv[j], "beginswith")) {
                selector = beginswitch(current_line, atoi(argv[j + 1]), argv[j + 2], delims);
                break;
            }
            if (!strcmp(argv[j], "contains")) {
                selector = contains(current_line, atoi(argv[j + 1]), argv[j + 2], delims);
                break;
            }
        }
        if (selector == 1) {
            for (j = 0; j < argc; j++) {
                if (strcmp(argv[j], "cset") == 0) {
                    if (flag_cset == 1) {
                        cset(atoi(argv[j + 1]), delims, argv[j + 2], current_line, &flag_print);
                        break;
                    }
                }
                if (strcmp(argv[j], "move") == 0) {
                    move(atoi(argv[j + 1]), atoi(argv[j + 2]), current_line, delims, &flag_print);
                    break;
                }
                if (strcmp(argv[j], "swap") == 0) {
                    swap(atoi(argv[j + 1]), atoi(argv[j + 2]), &flag_print, current_line, delims);
                    break;
                }
                if (strcmp(argv[j], "copy") == 0) {
                    copy(atoi(argv[j + 1]), atoi(argv[j + 2]), current_line, delims, &flag_print);
                    break;
                }
                if (!strcmp(argv[j], "round")) {
                    roundd(atoi(argv[j + 1]), current_line, delims, &flag_print);
                    break;
                }
                if (!strcmp(argv[j], "int")) {
                    to_int(atoi(argv[j + 1]), current_line, delims, &flag_print);
                    break;
                }
                if ((strcmp(argv[j], "tolower") == 0))  {
                    to_lower(atoi(argv[j + 1]), current_line, delims, &flag_print);
                    break;
                }

                if ((strcmp(argv[j], "toupper") == 0)) {
                    to_upper(atoi(argv[j + 1]), current_line, delims, &flag_print);
                    break;
                }
            }
        }
            // Checkovanie delimiter - int count_col - current column
             for (i = 0; i < line_length; i++) {
                
                if (count_col_char > MAX_COL) {
                    fprintf(stderr, "Prekrocena max velkost stlpca");
                }
                // Argument and function check
                for (j = 1; j < argc; j++) {

                    if ((strcmp(argv[j], "irow") == 0) && (count_line == (atoi(argv[j + 1])))) {
                        irow(atoi(argv[j + 1]), count_line, total_col, delims);
                        count_line++;
                        break;
                    }

                    if (strcmp(argv[j], "drow") == 0) {
                        flag_print = drow(atoi(argv[j + 1]), count_line);
                    }

                    if (strcmp(argv[j], "drows") == 0) {
                        flag_print = drows(atoi(argv[j + 1]), atoi(argv[j + 2]), count_line);
                    }

                    if (strcmp(argv[j], "icol") == 0) {
                        icol(atoi(argv[j + 1]), count_col,delims, &count_col);
                    }

                    flag_arow = arow(argv[j]);

                    if (strcmp(argv[j], "acol") == 0) {
                        flag_acol = 1;
                    }

                    if (strcmp(argv[j], "dcol") == 0) {
                        dcol(atoi(argv[j + 1]), count_col, current_line[i], &flag_print);
                    }

                    if (strcmp(argv[j], "dcols") == 0) {
                        dcols(atoi(argv[j + 1]), atoi(argv[j + 2]), count_col, current_line[i], &flag_print);
                    }
                   

                }
                if (current_line[i] == delims[0]) {
                    count_col++;
                    count_col_char = 0;
                }
                count_col_char++;
                if (flag_print == 1) {
                    fprintf(stdout, "%c", current_line[i]);
                }
                if ((flag_acol == 1) && (i == line_length - 2)) {
                    fprintf(stdout, "%c", delims[0]);
                }
            }

    }

    if (flag_arow == 1) {
       
        print_cols(total_col, delims);
        count_line++;
    }

    return 0;
}

/*END OF MAIN FUNCTION*/


// Return value is true if EOF is reached
bool is_last() {
    char temp = fgetc(stdin);
        if (temp == EOF){
        return true;
    }
    else {
        ungetc(temp, stdin);
        return false;
    }
}
// Letters in col C are transformed to lower letters
void to_lower(int C ,char* current_line, char* delims, int* ptr) {
    *ptr = 0;
    int count = 1;
    int i;
    int len = strlen(current_line);
    for (i = 0; i < len; i++) {
        if (current_line[i] == delims[0]) {
            count++;
            fprintf(stdout, "%c", current_line[i]);
        }
        else if (count == C && current_line[i] != delims[0]) {
            current_line[i] = tolower(current_line[i]);
        }
        if (current_line[i] != delims[0]) {
            fprintf(stdout, "%c", current_line[i]);
        }
    }
    return;
}
// Letter in col C are transformed to upper letters
void to_upper(int C, char* current_line, char* delims, int* ptr) {
    *ptr = 0;
    int count = 1;
    int i;
    int len = strlen(current_line);
    for (i = 0; i < len; i++) {
        if (current_line[i] == delims[0]) {
            count++;
            fprintf(stdout, "%c", current_line[i]);
        }
        else if (count == C && current_line[i] != delims[0]) {
            current_line[i] = toupper(current_line[i]);
        }
        if (current_line[i] != delims[0]) {
            fprintf(stdout, "%c", current_line[i]);
        }
    }
    return;
}

// Prints the char *temp with rounded / 'inted' num in col C
void print_temp_int(int C, int tmp, char* current_line, char* delims) {
    int i;
    bool check = true;
    int count = 1;
    int len = strlen(current_line);
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == delims[0]) {
            count++;
            fprintf(stdout, "%c", delims[0]);
            check = true;
        }
        if (count == C && check && current_line[i] != delims[0]) {
            fprintf(stdout, "%d", tmp);
            check = false;
        }
        if (current_line[i] != delims[0] && check) {
            fprintf(stdout, "%c", current_line[i]);
        }
    }
    fprintf(stdout, "\n");
}
/*Return value is 1, if column C contains the characters of search, else 0*/
int contains(char* current_line, int C, char* search, char* delims) {
    int count = 1;
    int len = strlen(current_line);
    int i;
    int j = 0;
    char temp[100];
    for (i = 0; i < len; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
        if (count == C && current_line[i] != delims[0]) {
            temp[j] = current_line[i];
            j++;
        }
    }
    temp[j] = '\0';
    if (strstr(temp, search) != NULL) {
        return 1;
    }
    else {
        return 0;
    }
}
/*Return value is 1, if the first characters of column C
 are the same as characters of search, otherwise 0   
*/
int beginswitch(char* current_line, int C, char* search, char *delims) {
    int count = 1;
    int len = strlen(current_line);
    int i;
    int j = 0;
    char temp[100];
    for (i = 0; i < len; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
        if (C == count && current_line[i] != delims[0]) {
            temp[j] = current_line[i];
            j++;
        }
    }
    temp[j] = '\0';
    len = strlen(search);
    for (i = 0; i < len; i++) {
        if (temp[i] != search[i]) 
        {
            return 0;
        }
    }
    return 1;
}
// Return value is 1 if count row >= N and row <= M
// Return value is 1 if N && M are - and bool last is true
// Return value is 1 if M is - and count row is >= N
int rows(int count_row, char* arg_N, char* arg_M, bool last) {
    int N = 1;
    int M = 1;
    if (!strcmp(arg_N, "-") && !strcmp(arg_M, "-")) {
        if (last) {
            return 1;
        }
        else {
            return 0;
        }
    }
    if (strcmp(arg_N,"-") != 0) {
        N = atoi(arg_N);
    }
    if (strcmp(arg_M, "-") != 0) {
        M= atoi(arg_M);
    }
    if (count_row >= N && !strcmp(arg_M, "-")) {
        return 1;
    }
    if (count_row >= N && count_row <= M) {
        return 1;
    }
    else {
        return 0;
    }
}
void move(int N, int M, char* current_line, char* delims, int* ptr) {
    *ptr = 0;
    
    int len = strlen(current_line);
    int count = 1;
    int i;
    int j = 0;
    char temp[100];
    char del = delims[0];
    bool check = true;
    // Record col C to char *tmp
    temp[0] = '\0';
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == del) {
            count++;
        }
        if (count == N && current_line[i] != del) {
            temp[j] = current_line[i];
            j++;
        }
    }

    temp[j] = '\0';
    count = 1;
    for (i = 0; i < len - 1; i++) {
        
        if (count == M && check) {
            fputs(temp, stdout); 
            fprintf(stdout, "%c", del); 
            check = false;
        }
        
        if (count != N && current_line[i] != del) {
            fprintf(stdout, "%c", current_line[i]);
        }
        if (current_line[i] == del) {
            if (count != N) {
                fprintf(stdout, "%c", del);
            }
            count++;
        }

    }
    fprintf(stdout, "\n");
}
// Replace col M with col C
// 1. cycle records N to tmp, and 2nd cycle prints tmp instead of col M
void copy(int N, int M, char* current_line, char* delims, int* ptr) {
    *ptr = 0;
    bool print = true;
    int i; 
    int j;
    char temp[MAX_COL];
    temp[0] = '\0';
    int count = 1;
    int t = 0;
    int len = strlen(current_line);
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
        else if (count == N) {
            temp[t] = current_line[i];
            t++;
        }
    }
    count = 1;
    temp[t] = '\0';
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == delims[0] && i != 0) {
            fprintf(stdout, "%c",current_line[i]);
            count++;
            print = true;
        }
        if (count == M && print) {
            for (j = 0; j <= t; j++) {
                if (temp[j] != delims[0] && temp[j] != '\0')
                fprintf(stdout, "%c", temp[j]);
            }
            print = false;
        }
        if (print && current_line[i] != delims[0]) {
            fprintf(stdout, "%c",current_line[i]);
        }
        if (current_line[i] == delims[0] && i == 0) {
            fprintf(stdout, "%c", current_line[i]);
            count++;
            print = true;
        }
    }
    fprintf(stdout, "\n");
}


// Swap of cols
// 1st cycle puts N and M to temp_1 and temp_2
// 2nd cycle prints N instead of M and M instead of M
void swap(int N, int M, int* ptr, char* current_line, char* delims) {
    *ptr = 0;
    int i;
    int count = 1;
    int len = strlen(current_line);
    int t1 = 0;
    int t2 = 0;
    char temp_1[MAX_COL];
    char temp_2[MAX_COL];
    temp_1[0] = '\0';
    temp_2[0] = '\0';
    for (i = 0; i < len -1  ; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
        if (count == N) {
            temp_1[t1] = current_line[i];
            t1++;
        }
        if (count == M) {
            temp_2[t2] = current_line[i];
            t2++;
        }
    }
    int j;
    temp_2[t2 + 1] = '\0';
    temp_1[t1 + 1] = '\0';
    count = 1;
    bool check = true;
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == delims[0] && i != 0) {
            check = true;
            count++;
            fprintf(stdout, "%c", current_line[i]);
        }
        if (count == N && check) {
            for (j = 0; j < t2; j++) {
                if (temp_2[j] != delims[0] && temp_2[j] != '\0') {
                    fprintf(stdout, "%c", temp_2[j]);
                }
            }
            check = false;
        }
        if (count == M && check) {
            for (j = 0; j < t1 ; j++) {
                if (temp_1[j] != delims[0] && temp_1[j] != '\0') {
                    fprintf(stdout, "%c", temp_1[j]);
                }
            }
            check = false;
        }
        
        if (check && current_line[i] != delims[0]) {
            fprintf(stdout, "%c", current_line[i]);
        }
        if (current_line[i] == delims[0] && i == 0) {
            check = true;
            count++;
            fprintf(stdout, "%c", current_line[i]);
        }
    }
    fprintf(stdout, "\n");
}
    
// Rounds number in col C
// If there isnt number in Col C program ignores it
void roundd(int C, char* current_line, char* delims, int* flag_print) {
    *flag_print = 0;
    int i;
    int j = 0;
    char tmp[100];
    int count = 1;
    int len = strlen(current_line);
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
        if (C == count && current_line[i] != delims[0]) {
            tmp[j] = current_line[i];
            j++;
        }
    }
    tmp[j] = '\0';
    char* ptr;
    double tmp_int = strtod(tmp, &ptr);
    if (*ptr == '\0') {
        print_temp_int(C, (int)(0.5 + tmp_int), current_line, delims);
    }
    else {
        fputs(current_line, stdout);
    }
    return;

}

// Makes an int from float/double
// If there is no float/double in col C, program ignores it 
void to_int(int C, char* current_line, char* delims, int* flag_print) {
    *flag_print = 0;
    int i;
    int j = 0;
    char tmp[100];
    int count = 1;
    int len = strlen(current_line);
    for (i = 0; i < len - 1; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
        if (C == count && current_line[i] != delims[0]) {
            tmp[j] = current_line[i];
            j++;
        }
    }
    tmp[j] = '\0';
    char* ptr;
    double tmp_double = strtod(tmp, &ptr);
    if (*ptr == '\0') {
        print_temp_int(C, (int)(tmp_double), current_line, delims);
    }
    else {
        fputs(current_line, stdout);
    }
    return;
}

// Deletes row arg_line
int drow(int arg_line, int count_line) {
    if (arg_line == count_line) {
        return 0;
    }
    else {
        return 1;
    }
}

void print_base(char* base) {
    if (base != NULL) {
        fprintf(stdout, "%s", base);
    }
}

// Deleting rows N  to M
int drows(int arg_N, int arg_M, int count_line) {
    if (arg_N == arg_M) {
        drow(arg_N, count_line);
    }
    if ((count_line >= arg_N) && (count_line <= arg_M)) {
        return 0;
    }
    else return 1;
}
// Finds and changes all delimiters to the first
// char of delimiters
void change_del(char* delims, char* current_line) {
    int len = strlen(current_line);
    int len_del = strlen(delims);
    char delim = delims[0];
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len_del; j++) {
            if (current_line[i] == delims[j]) {
                current_line[i] = delim;
            }
        }
    }

    return;
}

// Deletes col C
void dcol(int argv_C, int count_col,char current_char, int* ptr_flag) {
    if (argv_C == count_col && current_char != '\n') {
        *ptr_flag = 0;
    }
    else {
        *ptr_flag = 1;
    }
}
// Deletes cols N to M
void dcols(int arg_N, int arg_M, int count_col,char current_char, int* ptr) {
    if (arg_N == arg_M) {
        dcol(arg_N, count_col,current_char, ptr);
    }
    else if ((count_col >= arg_N) && (count_col <= arg_M)) {
        *ptr = 0;
    }
    else {
        *ptr = 1;
    }
    if (current_char == '\n') {
        *ptr = 1;
    }
}
// Inserts new column before column arg_C
void icol(int arg_C, int count_col,char *delims, int* ptr) {
    if (arg_C == count_col) {
        fprintf(stdout, "%c",delims[0]);
        *ptr = count_col + 1;
    }
}
// Prints new line after the last line of text file
int arow(char* arg_line) {
    if (strcmp(arg_line, "arow") == 0) {
        
        return 1;
    }
    else {
        return 0;
    }
}

// Prints new line before line arg_C
void irow(int arg_C, int count_line, int total_col, char *delims) {
    int i;
    if (arg_C == count_line) {
        for (i = 0; i < total_col - 1; i++) {
            fprintf(stdout, "%c", delims[0]);
        }
        fprintf(stdout, "\n");
    }
    return;
}

// Prints string to col arg_col
void cset(int arg_col, char* delims, char* argv, char* current_line, int* ptr){
    char del = delims[0];
    int count = 1;
    *ptr = 0;
    bool check = true;
    int i;
    int len = strlen(current_line);
    for (i = 0; i < len - 1; i++) {
        
        if (arg_col == count && check) {
            fprintf(stdout, "%s",argv);
            check = false;
        }
        if (current_line[i] == del) {
            fprintf(stdout, "%c", current_line[i]);
            count++;
        }

        if (current_line[i] != del && count != arg_col) {
            fprintf(stdout, "%c", current_line[i]);
        }
    }
    fprintf(stdout, "\n");
    return;
}

// Returns the number of cols in a line
int num_of_cols(char *current_line, char *delims) {
    int count = 1;
    int len = strlen(current_line);
    for (int i = 0; i < len; i++) {
        if (current_line[i] == delims[0]) {
            count++;
        }
    }
    return count;
}

// Prints the exact number of cols
// Used in irow, arow
void print_cols(int total_col, char* delims) {
    for (int i = 0; i < total_col - 1; i++) {
        fprintf(stdout,"%c",delims[0]);
    }
    fprintf(stdout, "\n");
    return;
}

// Return value is true if num > 0
// Used in error handling
bool is_greater(int num) {
    if (num < 1) {
         fprintf(stderr, "Input number cannot be <= 0\n");
         return false;
    }   
    else {
         return true;
    }
}
// Return value is true, if input arguments are correct
bool check_rows(char* argv[], int j) {
    char* ptr1 = NULL;
    char* ptr2 = NULL;
    int num1, num2;
    if (!strcmp(argv[j + 1],"-") && !strcmp(argv[j + 2], "-")) {
        return true;
    }
    if (!strcmp(argv[j + 1], "-") && strcmp(argv[j + 2], "-") != 0) {
        fprintf(stderr, "N  in rows can be - only if M is also -\n");
        return false;
    }
    if (!strcmp(argv[j + 2], "-")) {
        num1 = strtol(argv[j + 1], &ptr1, 10);
        if (*ptr1 == '\0' && num1 > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    num1 = strtol(argv[j + 1], &ptr1, 10);
    num2 = strtol(argv[j + 2], &ptr2, 10);
    if (*ptr1 == '\0' && *ptr2 == '\0') {
        if (num1 > num2) {
            fprintf(stderr, "First number in rows cannot be greater than second\n");
            return false;
        }
        if (num1 < 1 || num2 < 1) {
            fprintf(stderr, "Input number cannot be <= 0\n");
            return false;
        }
    }
    else {
        fprintf(stderr, "Input argument in rows must be number or  -  \n");
        return false;
    }
    return true;
}


/*ERROR HANDLING BOOLS*/
bool error0(const char* check_2[], int argc, char* argv[]) {
    int i;
    int j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < argc; j++) {
            if (strcmp(check_2[i], argv[j]) == 0) {
                return false;
            }
        }
    }
    return true;
}
/*Check if argument has the expected amount of arguments after him */
bool error1(int argc,int j,char* argv[]) {
    if (j + 1 < argc && (isdigit(*argv[j + 1]) || *argv[j + 1] == '-')) {
        return true;
    }
    else {
        fprintf(stderr, "Chyba v pocte argumentov funkcie\n");
        return false;
    }
}
/*Check if argument has the expected amount of arguments after him */
bool error2(int argc, int j, char* argv[], int i) {
    // 14 is the starting position of args with 1st
    // arg as number and second arg as string in check_2
    char* ptr1 = NULL;
    char* ptr2 = NULL;
    int num1;
    int num2;
    if (argc - (j + 1) < 2) {
        fprintf(stderr, "Too few arguments");
        return false;
    }
    if (strcmp(argv[j], "rows") == 0) {
        if (!check_rows(argv, j)) {
            return false;
        }
    }
    if (i > 13) {
        num1 = strtol(argv[j + 1], &ptr1, 10);
        if (*ptr1 == '\0') {
            if (num1 < 1) {
                fprintf(stderr, "Input num cannot be lower than 1");
                return false;
            }
        }
        else {
            fprintf(stderr, "1st argument must be a number");
            return false;
        }
    }
    if (i < 13) {
        num1 = strtol(argv[j + 1], &ptr1, 10);
        num2 = strtol(argv[j + 2], &ptr2, 10);
        if (*ptr1 == '\0' && *ptr2 == '\0') {
            if (i < 10) {
                if (num1 > num2) {
                    fprintf(stderr, "First number cannot be greater than second\n");
                    return false;
                }
            }
            if (num1 < 1 || num2 < 1) {
                fprintf(stderr, "Input number cannot be <= 0\n");
                return false;
            }
        }
        else {
            fprintf(stderr, "Input must be a number!\n");
        }
    }
    return true;
}


// Check of input data
// If unexpected input data - return value is false, and error is printed
bool check_args(const char* check[], int argc, char *argv[]) {
    if (!strcmp(argv[1], "-d") && argc <= 2) {
        fprintf(stderr,"Problem with argument -d -- delimiters are non existent\n");
        return false;
    }
    
    int i;
    int j;
    for (i = 0; i < NUM_OF_FC ; i++) {
        for (j = 0; j < argc; j++) {
            if (strcmp(check[i], argv[j]) == 0) {
                if (i < 8) {
                    if(!error1(argc, j,argv)) {
                        return false;
                    }
                    if (is_greater(strtol(argv[j + 1], NULL, 10)) == false) {
                        return false;
                    }
                   
                }
               
                else {
                    
                    if (!error2(argc, j, argv, i)) {
                        return false;
                    }
                 
                }
                
            }
        }
    }
    return true;
}

/* END OF ERROR HANDLING BOOLS */