/*
2x3x4
+----------------------------------------+
|+------------------++------------------+|
||+----++----++----+||+----++----++----+||
|||1234||1234||1234||||1234||1234||1234|||
||+----++----++----+||+----++----++----+||
|+------------------++------------------+|
+----------------------------------------+

+------+
|abcdef| 6 pointers to 1's ( array of row pointers )
+------+

+--+
|mn| 2 pointers to a and d above ( array of pointers to slices )
+--+

a[i][j][k]
i chooses which slice
j chooses which row within the slice
k chooses within element with the row
*/

#include "tddaa.h"
#include <stdlib.h>

int ***allocate(int d1, int d2, int d3)
{
    int i, j;
    int *column = (int *)malloc(d1 * d2 * d3 * sizeof(int));
    int **row = (int **)malloc(d1 * d2 * sizeof(int *));
    int ***slice = (int ***)malloc(d1 * sizeof(int **));

    if (slice && row && column)
    {

        for (i = 0; i < d1; i++)
        {
            slice[i] = row + i * d2;
            for (j = 0; j < d2; j++)
            {
                slice[i][j] = column + (j + (d2 * i)) * d3;
            }
        }

        /*printf("%p | %p\n", &slice[0][1][0], &slice[1][1][0]);*/
        return slice;
    }

    free(column);
    free(row);
    free(slice);
    return NULL;
}

void deallocate(int ***ppp)
{
    free(**ppp);
    free(*ppp);
    free(ppp);
}
