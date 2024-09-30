#include "catalan.h"

int catalan(int, int)
{
    return 0;
}

int catalan2(int size)
{
    if (size < 2)
        return 1;

    int n = 0;
    for (int s = 0; s < size; ++s)
    {
        //    left           right    (-1 is for root)
        //    subtree        subtree
        n += (catalan2(s) * catalan2(size - 1 - s));
    }
    return n;
}

int catalan3(int size)
{
    if (size < 2)
    {
        return 1;
    }

    int n = 0;

    for (int s = 0; s < size; s++)
    {
        //    left tree     middle tree          right tree
        n += (catalan3(s) * catalan3(size - s) * catalan3(1));
    }

    return n;
}

int catalan4(int)
{
    return 0;
}
