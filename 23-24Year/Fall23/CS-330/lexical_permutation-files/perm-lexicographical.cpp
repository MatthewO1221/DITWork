#include "perm-lexicographical.h"
#include <algorithm>
#include <iostream>

bool nextPermLexicographical(std::vector<int> &p)
{
    /*
    1. Scan it right to left to find the first pair of elements such that ai < ai+1.
2. Insert all elements from {a0, . . . , ai−1} into pj+1.
3. Find the smallest ak within the “tail” (ai+1, . . . , an) such that it ak > ai
.
4. Insert ak at the end of pj+1.
5. Insert at the end of pj+1 all remaining elements in increasing order.
    */
}
