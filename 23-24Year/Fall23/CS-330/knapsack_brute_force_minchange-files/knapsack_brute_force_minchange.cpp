#include "knapsack_brute_force_minchange.h"
#include <iostream>

#define DEBUGMODE 0

////////////////////////////////////////////////////////////////////////////////

GreyCode::GreyCode(int s)
    : curPermNum(0), curPermRep(s, false)
{
}

std::pair<bool, std::pair<bool, int>> GreyCode::Next()
{
    int pos;           // which position is modified
    bool add;          // is true if new value is 1 (add item), false otherwise
    bool last = false; // is this the last permutation
    // If we are on an even permutation
    if (curPermNum % 2 == 0)
    {
        pos = 0;

        // Flip to true every 4th permutation, starting with first
        if (curPermNum % 4 == 0)
        {
            add = true;
        }
        else
        {
            add = false;
        }

        curPermRep[0] = !curPermRep[0];
    }
    else
    {
        int i = 0;
        for (; i < static_cast<int>(curPermRep.size()) - 1; i++)
        {
            if (curPermRep[i] == true)
            {
                break;
            }
        }

        pos = i + 1;
        add = !curPermRep[i + 1];

        curPermRep[i + 1] = !curPermRep[i + 1];
    }
    curPermNum++;
    if (curPermNum + 1 == 1 << (static_cast<int>(curPermRep.size())))
    {
#if DEBUGMODE
        std::cout << "Perm number: " << curPermNum << " is the last perm" << std::endl;
#endif
        last = true;
    }

    return std::make_pair(!last, std::make_pair(add, pos));
}

////////////////////////////////////////////////////////////////////////////////
std::vector<bool> knapsack_brute_force(std::vector<Item> const &items, Weight const &W)
{
    std::vector<bool> solution(items.size(), false);
    std::vector<bool> curSolution(items.size(), false);
    GreyCode curPerm(items.size());
    Weight curWeight;
    bool go = true;

    while (go)
    {
        std::pair<bool, std::pair<bool, int>> permSpecifics = curPerm.Next();
        go = permSpecifics.first;

        if (permSpecifics.second.first)
        {
            curWeight += items[permSpecifics.second.second].GetWeight();
            curSolution[permSpecifics.second.second] = true;
        }
        else
        {
            curWeight -= items[permSpecifics.second.second].GetWeight();
            curSolution[permSpecifics.second.second] = false;
        }

#if DEBUGMODE

        std::cout << "Checking solution: ";
        for (int i = 0; i < static_cast<int>(curSolution.size()); i++)
        {
            std::cout << curSolution[i];
        }
        std::cout << std::endl;

#endif

        if (curWeight <= W && compareSolution(solution, curSolution, items))
        {
            solution = curSolution;
        }
    }

    return solution;
}

bool compareSolution(std::vector<bool> curSol, std::vector<bool> newSol, std::vector<Item> items)
{
    int curValue = 0;
    int newValue = 0;

    for (int i = 0; i < static_cast<int>(curSol.size()); i++)
    {
        if (curSol[i])
        {
            curValue += items[i].GetValue();
        }

        if (newSol[i])
        {
            newValue += items[i].GetValue();
        }
    }

#if DEBUGMODE
    std::cout << "Old solution had value of: " << curValue << " new solution has value of: " << newValue << std::endl;
#endif

    return newValue > curValue;
}