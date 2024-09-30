#include "perm-jt.h"
#include <iostream>

#define DEBUGMODE 0
PermJohnsonTrotter::PermJohnsonTrotter(int size)
    : dir(), values()
{

    for (int i = 1; i < size + 1; i++)
    {
        values.push_back(i);
        dir.push_back(Left);
    }
}

std::vector<int> const &PermJohnsonTrotter::Get() const
{
    return values;
}

bool PermJohnsonTrotter::isMobile(int index)
{
    if (dir[index] == Left && index != 0)
    {
        return values[index] > values[index - 1];
    }
    else if (dir[index] == Right && index < static_cast<int>(values.size()) - 1)
    {
        return values[index] > values[index + 1];
    }

    return false;
}

int PermJohnsonTrotter::findLargestMobile()
{
    int largestValue = 0;
    int largestIndex = -1;

    for (int i = 0; i < static_cast<int>(values.size()); i++)
    {
        if (values[i] > largestValue && isMobile(i))
        {
            largestValue = values[i];
            largestIndex = i;
        }
    }

#if DEBUGMODE
    if (largestIndex == -1)
    {
        std::cout << "No mobile values" << std::endl;
    }
    else
    {
        std::cout << "Current largest mobile: " << values[largestIndex] << " with direction: " << static_cast<int>(dir[largestIndex]) << std::endl;
    }

#endif
    return largestIndex;
}

void PermJohnsonTrotter::swapGreater(int value)
{
#if DEBUGMODE
    std::cout << "Swapping direction of all values greater than: " << value << std::endl;
#endif
    for (int i = 0; i < static_cast<int>(values.size()); i++)
    {
        if (values[i] > value)
        {
            if (dir[i] == Left)
            {
#if DEBUGMODE
                std::cout << "Swapping from left to right on value: " << values[i] << std::endl;
#endif
                dir[i] = Right;
            }
            else
            {
#if DEBUGMODE
                std::cout << "Swapping from right to left on value: " << values[i] << std::endl;
#endif
                dir[i] = Left;
            }
        }
    }
}

bool PermJohnsonTrotter::Next()
{

    int largestMobile = findLargestMobile();

    if (largestMobile == -1)
    {
        return false;
    }

    int largestDirection = dir[largestMobile];

    std::swap(values[largestMobile], values[largestMobile + largestDirection]);
    std::swap(dir[largestMobile], dir[largestMobile + largestDirection]);

    swapGreater(values[largestMobile + largestDirection]);

    return true;
}