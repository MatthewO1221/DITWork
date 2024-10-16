#include "tddaa.h"
#include <stdio.h>

void run(int, int, int);
void test0(void);
void test1(void);

void run(int d1, int d2, int d3)
{
    /* printf( "%dx%dx%d\n", d1,d2,d3 ); */
    int i, j, k;
    char ok = 1;

    int ***ppp = allocate(d1, d2, d3);

    for (i = 0; i < d1; ++i)
    {
        for (j = 0; j < d2; ++j)
        {
            for (k = 0; k < d3; ++k)
            {
                        

                ppp[i][j][k] = 100 * i + 10 * j + k; /* 3-digit number ijk if i,j,k are single digits */
                /*printf("Data at [%d][%d][%d] is %d\n", i, j, k, ppp[i][j][k]);*/
            }
        }
    }
    /*printf("%p | %p\n", &ppp[0][0][0], &ppp[1][0][0]);*/
    for (i = 0; i < d1; ++i)
    {
        for (j = 0; j < d2; ++j)
        {
            for (k = 0; k < d3; ++k)
            {
                
                if (ppp[i][j][k] != 100 * i + 10 * j + k)
                {
                    printf("Error: ");
                    ok = 0;
                }
                /*printf("Data at [%d][%d][%d] should be %d | ", i, j, k, 100 * i + 10 * j + k);
                printf("Data at [%d][%d][%d] is %d\n", i, j, k, ppp[i][j][k]);*/
            }
        }
    }

    if (ok)
    {
        printf("OK\n");
    }

    deallocate(ppp);
}

void test0(void)
{
    run(2, 3, 4);
}

#include <time.h>   /* time */
#include <stdlib.h> /* rand/srand */
void test1(void)
{
    srand(time(0));
    run(rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
}

void (*pTests[])() = {test0, test1};

#include <stdio.h> /* sscanf */
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        int test = 0;
        sscanf(argv[1], "%i", &test);
        pTests[test]();
    }
    return 0;
}
