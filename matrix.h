#ifndef MATRIX_H
#define MATRIX_H

#include<stdbool.h>

typedef struct matrix{
  int n;
  int m;
  int** data;
} matrix;


bool init_matrix(matrix* mat, char* filename);

void alloc_matrix(matrix* mat);
void dealloc_matrix(matrix* mat);

void print_matrix(matrix* mat);
void write_matrix(matrix* mat, char* filename);

#endif