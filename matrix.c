#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// checks if string contains digits only
bool isnumber(char* str){ 
    int len = strlen(str);

    int i = 0;
    if (str[0] == '-')
        i++;

    for (;i < len;i++){
        if (!isdigit(str[i]))
            return false;
    }

    return true;
}

// reads matrix entries from filename and stores them in mat
// returns true if succeeded
bool get_data(matrix* mat, char* filename){
    alloc_matrix(mat);  // allocate data for matrix

    FILE* file = fopen(filename, "r");
    int line_length = mat -> m * 10; // ~ 10 chars for each column 
    char line[line_length]; 
    fgets(line, 100, file); // get the dimension line

    for (int i = 0;i < mat -> n;i++){

        if (fgets(line, line_length, file) == NULL){ // get a row of data
            printf("Not engough rows: %s\n", filename);
            fclose(file);
            return false;
        } 

        char delimiters[] = " \t\n";
        char* token = strtok(line, delimiters);  // get tokens from line
        for (int j = 0;j < mat -> m;j++){
            if (token == NULL){ // no more tokens left
                printf ("Not enough numbers on row %d\n", i);
                fclose(file);
                return false;
            }
            
            if (!isnumber(token)){ // NaN encountered
                printf("Found \"%s\"  not a number\n", token);
                fclose(file);
                return false;
            }
                
            mat -> data[i][j] = atoi(token); 
            token = strtok(NULL, delimiters); // get next token
        }
    }

    fclose(file);
    return true;
}

//  populate the n, m fields of "mat" from "filename"
// returns true if succeeded
bool get_dimension(matrix* mat, char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("Couldn't open file %s\n", filename);
        return false;
    }

    int n, m;

    
    if (fscanf(file, "row =%d col =%d\n", &n, &m) < 2 || n <= 0 || m <= 0){  // Throw error if less than two arguments were read (n and m)
        printf("Invalid dimension format or value: %s\n", filename);
        fclose(file);
        return false;
    }

    mat -> n = n;
    mat -> m = m;
    fclose(file);

    //printf("Dimension %s : %d %d\n", filename, mat -> n, mat -> m);
    return true;
}


// returns true if matrix was initialized successfuly 
bool init_matrix(matrix* mat, char* filename){
    return get_dimension(mat, filename) &&
           get_data(mat, filename);
}


// allocate space for matrix data
void alloc_matrix(matrix* mat){
    // allocate an array of pointers, each pointer points at a row of the matrix
    mat -> data = malloc((mat -> n) * sizeof(int*));
    
    // allocate space for the data itself  
    mat -> data[0] = malloc((mat -> n) * (mat -> m) * sizeof(int));

    for (int i = 0; i < mat -> n; i++)
        mat -> data[i] = mat -> data[0] + (i * (mat -> m));  // data[i] points at start of row i
    
}

// free data if it was allocated
void dealloc_matrix(matrix* mat){
    if (mat -> data != 0)   // data was initialized
        free(mat -> data[0]);
    free(mat -> data);
}

void print_matrix(matrix* mat){
    for (int i = 0;i < mat -> n;i++){
        for (int j = 0;j < mat -> m;j++)
            printf("%d\t", mat -> data[i][j]);
        
        printf("\n");
    }
}

// write matrix to file
void write_matrix(matrix* mat, char* filename){
    FILE* file = fopen(filename, "w");
    for (int i = 0;i < mat -> n;i++){
        for (int j = 0;j < mat -> m;j++)
            fprintf(file, "%d\t", mat -> data[i][j]);
        
        fprintf(file, "\n");
    }

    fclose(file);
}