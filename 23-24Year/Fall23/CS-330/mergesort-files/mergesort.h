#ifndef MERGESORT_H
#define MERGESORT_H

#include <iostream>
/**
 * @brief Performs recursive mergesort on given array
 *
 * @param a The array to sort
 * @param p Beginning index of array
 * @param r One past end index of array
 */
void mergesort(int *a, unsigned p, unsigned r);

void merge(int *a, int left, int middle, int right);
#endif
