#ifndef KNAPSACK_BRUTE_FORCE_MINCHANGE
#define KNAPSACK_BRUTE_FORCE_MINCHANGE
#include <utility> // std::pair
#include <vector>
#include <bitset>
#include "definitions.h"

class GreyCode
{
private:
    int curPermNum;
    std::vector<bool> curPermRep;

public:
    GreyCode(int s);
    // first subset is empty, this function tells what's next 2^n-1 subsets
    // ret value: is last, is add, positin
    std::pair<bool, std::pair<bool, int>> Next();
};

// brute-force knapsack
std::vector<bool> knapsack_brute_force(std::vector<Item> const &items, Weight const &W);
bool compareSolution(std::vector<bool> curSol, std::vector<bool> newSol, std::vector<Item> items);
#endif
