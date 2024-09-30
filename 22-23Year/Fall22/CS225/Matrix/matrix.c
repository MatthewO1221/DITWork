#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void matrix_print(Matrix m, int num_rows, int num_columns)
{
    int i, j;
    for (i = 0; i < num_rows; ++i)
    {
        for (j = 0; j < num_columns; ++j)
        {
            printf("%4i ", m[i][j]);
        }
        printf("\n");
    }
}

Matrix matrix_create(int num_rows, int num_columns)
{
    Matrix newMatrix = malloc(sizeof(Matrix) * num_rows);
    if (newMatrix)
    {
        int i;
        for (i = 0; i < num_rows; i++)
        {
            
            newMatrix[i] = malloc(sizeof(int*) * num_columns);
        }
        
        return newMatrix;
    }

    return NULL;
}

void matrix_add(Matrix m1, Matrix m2, Matrix result, int num_rows, int num_columns)
{
    if (m1 && m2 && result)
    {
        int i, j;
        for (i = 0; i < num_rows; i++)
        {
            for (j = 0; j < num_columns; j++)
            {
                result[i][j] = m1[i][j] + m2[i][j];
            }
        }
    }
}

void matrix_delete(Matrix m, int num_rows)
{
    if (m)
    {
        int i;
        for (i = 0; i < num_rows; i++)
        {
            free(m[i]);
        }

        free(m);
    }
}

Matrix matrix_transpose(Matrix m, int num_rows, int num_columns)
{
    Matrix newMatrix = matrix_create(num_columns, num_rows);
    if (newMatrix)
    {
        int i, j;
        for (i = 0; i < num_rows; i++)
        {
            for (j = 0; j < num_columns; j++)
            {
                newMatrix[j][i] = m[i][j];
            }
        }

        return newMatrix;
    }

    return NULL;
}

void matrix_delete_row(Matrix m, int r, int num_rows)
{
    if (m)
    {
        int i;
        free(m[r]);
        for (i = r; i < num_rows; i++)
        {
            m[i] = m[i + 1];
        }
    }
}

void matrix_delete_column( Matrix m, int c, int num_rows, int num_columns)
{
    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        for (j = c; j < num_columns; j++)
        {
            m[i][j] = m[i][j + 1];
        }
        
    }
    
}