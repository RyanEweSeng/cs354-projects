/******************************************************************************
 * @file: latin_square_functions.c
 *
 * retoh
 * Ryan Toh
 * 908 160 7708
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 *
 * @creator: Ryan (retoh@wisc.edu)
 * @modified: SUBMISSION DATE
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "latin_square_functions.h"

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
// before submitting!
#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }
#define dbgprint_latin_square(n, LS) if (DEBUG) { Print_Latin_Square(n, LS); }

/******************************************************************************
 * Data Types and Structures
 *****************************************************************************/

// ADD ANY ADDITIONAL DATA TYPES OR STRUCTSvHERE 

/******************************************************************************
 * Globals
 *****************************************************************************/
extern void Print_Latin_Square(const size_t n, char **latin_square);

/******************************************************************************
 * Helper functions
 *****************************************************************************/

int seen(char ch, char *ptr, int size) {
    for (int i = 0; i < size; i++) {
        if (ch == *ptr) {
            return 1;
        } else {
            ptr++;
        }
    }

    return 0;
}

char * remove_newline(char *str) {
    char *res = str;
    while (*str != '\0') {
        if (*str == '\n') *str = '\0';
        str = str + 1;
    }

    return res;
}

char * copy(char *dest, char *src) {
    if (dest == NULL) return NULL;

    char *res = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    return res;
}

/******************************************************************************
 * Verificaiton functions
 *****************************************************************************/

/* 
 * This function takes the name of the file containing the latin square
 * and reads in the data to the the latin_square parameter.  
 *
 * There are many approaches that will work to read in the latin square data.
 * In any approach you choose, you will need to do at least the following:
 *     1) open the file 
 *     2) read in the text from the file
 *     3) figure out the dimensions of the latin square (n)
 *     4) reserve memory for the latin_square. This requires 2 steps
 *         4a) reserve an array of pointers to the rows
 *         4b) reserve an array of characters for each row
 *     5) fill in the latin_square data structure 
 *     6) close the file
 *
 * @param filename The name of the file to read in
 * @param latin_square_in A pointer to the latin square variable in main
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 */
void Read_Latin_Square_File(const char *filename, 
                            char ***latin_square_in, 
                            size_t *n) {
    /* BEGIN MODIFYING CODE HERE */
    FILE *file_ptr = fopen(filename, "r");
    int buffer_size = 2000;
    char *buffer = malloc(buffer_size * sizeof(char));

    if (file_ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fgets(buffer, buffer_size, file_ptr)) {
        *n = *n + 1;
    }
    *n = *n - 1;
    
    *latin_square_in = malloc(*n * sizeof(char *));
    char ** original_ptr = *latin_square_in;
    for (int i = 0; i < *n; i++) {
        **latin_square_in = malloc(*n * sizeof(char));
        *latin_square_in = *latin_square_in + 1;
    }
    *latin_square_in = original_ptr;

    rewind(file_ptr);

    for (int i = 0; i < *n; i++) {
        fgets(buffer, buffer_size, file_ptr);
        buffer = remove_newline(buffer);
        **latin_square_in = copy(**latin_square_in, buffer);
        *latin_square_in = *latin_square_in + 1;
    }
    *latin_square_in = original_ptr;

    fclose(file_ptr);
    free(buffer);
    /* END MODIFYING CODE HERE */
}

/* 
 * This function checks to see that exactly n symbols are used and that 
 * each symbol is used exactly n times.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Alphabet(const size_t n, char **latin_square) {
    if (latin_square == NULL) {
        printf("Verify_Alphabet - latin_square is NULL\n"); 
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */
    char *symbols = malloc(n * sizeof(char));
    char *symbol_ptr = symbols;
    int size = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            char ch = *(*(latin_square + row) + col);
            if (!seen(ch, symbols, size)) {
                *symbol_ptr = ch;
                symbol_ptr++;
                size++;
            }
        }
    }
    symbol_ptr = symbols;

    if (size != n) return 0;

    int freq;
    for (int i = 0; i < size; i++) {
        char ch = *symbol_ptr;
        freq = 0;
        for (int row = 0; row < n; row++) {
            for (int col = 0; col < n; col++) {
                char curr = *(*(latin_square + row) + col);
                if (ch == curr) freq++;
            }
        }
        if (freq != n) return 0;
        symbol_ptr++;
    }

    free(symbols);

    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function verifies that no symbol is used twice in a row or column.
 * It prints an error message alerting the user which rows or columns have 
 * duplicate symbols. This means that you will have at most n row error prints
 * and n column error prints.
 * 
 * Note: Test all rows first then test all columns.
 * 
 * Error messages have been included for you. Do not change the format out the 
 * print statements, as our grading scripts will use exact string matching. You
 * change the variable passed as the formatting argument to printf.
 * 
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
 int Verify_Rows_and_Columns(const size_t n, char **latin_square){
    if (latin_square == NULL) {
        printf("Verify_Rows_and_Columns - latin_square is NULL\n"); 
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */
    int is_valid = 1;
    // printf("Error in row %d\n", i);
    int row_flag;
    for (int row = 0; row < n; row++) {
        row_flag = 1;
        for (int i = 0; i < n; i++) {
            char curr_symbol = *(*(latin_square + row) + i);
            for (int j = i + 1; j < n; j++) {
                char curr_char = *(*(latin_square + row) + j);
                if (curr_char == curr_symbol) {
                    is_valid = 0;
                    row_flag = 0;
                    break;
                }
            }
            if (!row_flag) break;
        }
        if (!row_flag) printf("Error in row %d\n", row);
    }

    // printf("Error in column %d\n", i);
    int col_flag;
    for(int col = 0; col < n; col++) {
        col_flag = 1;
        for (int i = 0; i < n; i++) {
            char curr_symbol = *(*(latin_square + i) + col);
            for (int j = i + 1; j < n; j++) {
                char curr_char = *(*(latin_square + j) + col);
                if (curr_char == curr_symbol) {
                    is_valid = 0;
                    col_flag = 0;
                    break;
                }
            }
            if (!col_flag) break;
        }
        if (!col_flag) printf("Error in col %d\n", col);
    }

    if (!is_valid) {
        return 0;
    }
    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function calls free to allow all memory used by the latin_square 
 * verification program to be reclaimed.
 *
 * Note: you will have n+1 calls to free
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 */ 
void Free_Memory(const size_t n, char **latin_square) {
    /* BEGIN MODIFYING CODE HERE */
    char **original_ptr = latin_square;
    for (int i = 0; i < n; i++) {
        free(*latin_square);
        latin_square = latin_square + 1;
    }
    latin_square = original_ptr;
    free(latin_square);
    /* END MODIFYING CODE HERE */
}


