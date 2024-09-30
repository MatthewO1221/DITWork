#include "mergesort.h"
#include <algorithm>
#include <cstring>

#define DEBUG 1

int recursive = 0;

void mergesort(int *a, unsigned p, unsigned r)
{
    if (p < r)
    {
        unsigned int middle = p + (r - p) / 2;
        // recursive++;
        // std::cout << recursive << ": " << middle << std::endl;
        mergesort(a, p, middle);
        mergesort(a, middle + 1, r);

        merge(a, p, middle, r);
    }
}

void merge(int *a, int begin, int middle, int end)
{

    int *leftArray = new int[middle - begin + 1];
    int *rightArray = new int[end - middle];

    for (int i = 0; i < middle - begin + 1; i++)
    {
        leftArray[i] = a[begin + i];
    }

    for (int i = 0; i < end - middle; i++)
    {
        rightArray[i] = a[middle + 1 + i];
    }

#if DEBUG

    if (middle - begin + 1 < 50)
    {
        std::cout << "LEFT ARRAY: " << middle - begin + 1 << std::endl;
        for (int i = 0; i < middle - begin + 1; i++)
        {
            std::cout << leftArray[i] << std::endl;
        }

        std::cout << "RIGHT ARRAY: " << end - middle << std::endl;
        for (int i = 0; i < end - middle; i++)
        {
            std::cout << rightArray[i] << std::endl;
        }
    }

#endif

    int i1 = 0, i2 = 0;

    while (i1 < middle - begin + 1 && i2 < end - middle)
    {
        if (leftArray[i1] < rightArray[i2])
        {
            a[i1 + i2] = leftArray[i1];
            i1++;
        }
        else
        {
            a[i1 + i2] = rightArray[i2];
            i2++;
        }
    }

    while (i1 < middle - begin + 1)
    {
        a[i1 + i2] = leftArray[i1];
        ++i1;
    }
    while (i2 < end - middle)
    {
        a[i1 + i2] = rightArray[i2];
        ++i2;
    }

    delete[] leftArray;
    delete[] rightArray;
}