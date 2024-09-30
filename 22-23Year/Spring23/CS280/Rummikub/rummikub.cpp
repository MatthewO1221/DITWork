/**
 * @file rummikub.cpp
 * @author Matthew Oliver
 * @brief
 * @version 0.1
 * @date 2023-02-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include "rummikub.h"

/**
 * @brief Turning this on turns on various helpful prints, beware, this will fill files quickly
 *
 */
#define DEBUGMODE 0

/**
 * @brief Turning this on adds a final print of the number of iterations and the hand/groups/runs at the end of the recursion
 *
 */
#define SEEANSWER 1

static int iterations = 0;

std::vector<Tile> RummiKub::hand;
std::vector<std::vector<Tile>> RummiKub::groups;
std::vector<std::vector<Tile>> RummiKub::runs;

void RummiKub::Add(const Tile &tile)
{
    hand.push_back(tile);
}

void RummiKub::Solve()
{
    // Sort the hand in descending order by denomination
    std::sort(hand.begin(), hand.end(), std::greater<Tile>());

    // Call the recursive function with the end of the hand
    Solver(static_cast<int>(hand.size()) - 1);
#if SEEANSWER
    std::cout << "\n=====================================================\n";
    std::cout << "Iterations: " << iterations << std::endl;
    std::cout << "hand: ";
    for (auto const &t : hand)
    {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    for (auto const &g : groups)
    {
        std::cout << "(G( ";
        for (auto const &t : g)
        {
            std::cout << t << " ";
        }
        std::cout << " )))\n";
    }
    for (auto const &r : runs)
    {
        std::cout << "(R( ";
        for (auto const &t : r)
        {
            std::cout << t << " ";
        }
        std::cout << " )))\n";
    }
#endif
}

std::vector<std::vector<Tile>> RummiKub::GetGroups() const
{
    return groups;
}

std::vector<std::vector<Tile>> RummiKub::GetRuns() const
{
    return runs;
}

std::vector<RummiKub::Action> RummiKub::GetOptions(int index)
{
    std::vector<Action> actions;

    // Go through all the runs
    for (size_t i = 0; i < runs.size(); i++)
    {
        if (runs[i].size() + CountColorBefore(hand, index, hand[index].color) + 1 >= 3)
        {
            // If it is possible for this run to reach the minimum amount then add an action to add to it
            actions.push_back(Action(hand[index], static_cast<int>(i), AddtoRun));
        }
    }

    for (size_t i = 0; i < groups.size(); i++)
    {
        if (groups[i].size() + CountDenomination(hand, groups[i].back().denomination) >= 3)
        {
            // If it is possible for this group to reach the minimum amount then add an action to add to it
            actions.push_back(Action(hand[index], static_cast<int>(i), AddtoGroup));
        }
    }

    if (hand.size() >= 3)
    {
        if (CountColorBefore(hand, index, hand[index].color) + 1 >= 3)
        {
            // If a run using this tile could possibly reach the minimum amount then add an action to create a run
            actions.push_back(Action(hand[index], MakenewRun));
        }

        if (CountDenomination(hand, hand[index].denomination) >= 3)
        {
            // If a group using this tile could possibly reach the minimum amount then add an action to create a new group
            actions.push_back(Action(hand[index], MakenewGroup));
        }
    }

    return actions;
}

bool RummiKub::ActionIsLegal(Action action)
{
    if (action.option == AddtoGroup)
    {
        std::vector<Tile> group = groups[action.num];
#if DEBUGMODE
        std::cout << "Group Tile: " << group.back() << " Action Tile: " << action.tile << std::endl;
#endif
        // If the group is of the same denomination
        if (groups.at(action.num).back().denomination == action.tile.denomination)
        {
            for (size_t i = 0; i < groups.at(action.num).size(); i++)
            {
                // If this tile has the same color as any of the tiles in the group
                if (groups.at(action.num).at(i).color == action.tile.color)
                {
                    // This action is illegal
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    else if (action.option == AddtoRun)
    {
        std::vector<Tile> run = runs[action.num];
#if DEBUGMODE
        std::cout << "Run Tile: " << run.back() << " Action Tile: " << action.tile << std::endl;
#endif
        // If this tile has a value one greater than the end of the run and is the same color
        if (run.back().denomination == action.tile.denomination - 1 &&
            run.back().color == action.tile.color)
        {
            // This action is legal
            return true;
        }

        return false;
    }

    // If the action is to create a new group or run that is always legal
    return true;
}

bool RummiKub::AllLegal()
{
    return RunsLegal() && GroupsLegal();
}

bool RummiKub::RunsLegal()
{
    for (size_t i = 0; i < runs.size(); i++)
    {
        // Check legality of all runs
        if (!RunLegal(runs[i]))
        {
#if DEBUGMODE
            std::cout << "Run is illegal:" << std::endl;
            std::cout << "(R( ";
            for (auto const &t : runs[i])
            {
                std::cout << t << " ";
            }
            std::cout << " )))\n";
#endif
            return false;
        }
    }

    return true;
}

bool RummiKub::GroupsLegal()
{
    for (size_t i = 0; i < groups.size(); i++)
    {
        // Check legality of all groups
        if (!GroupLegal(groups[i]))
        {
#if DEBUGMODE
            std::cout << "Group is illegal:" << std::endl;
            std::cout << "(G( ";
            for (auto const &t : groups[i])
            {
                std::cout << t << " ";
            }
            std::cout << " )))\n";
#endif
            return false;
        }
    }

    return true;
}

bool RummiKub::RunLegal(std::vector<Tile> run)
{
    // If the run is less than the minimum size
    if (run.size() < 3)
    {
        return false;
    }

    Tile *lastTile = nullptr;
    for (size_t i = 0; i < run.size(); i++)
    {
        // If the denomination count is not consecutive at any point in the run
        if (lastTile && lastTile->denomination != run[i].denomination - 1)
        {
            return false;
        }
        lastTile = &run[i];
    }

    lastTile = nullptr;

    for (size_t i = 0; i < run.size(); i++)
    {
        // If any of the tiles are a different color than the rest
        if (lastTile && lastTile->color != run[i].color)
        {
            return false;
        }
        lastTile = &run[i];
    }

    return true;
}

bool RummiKub::GroupLegal(std::vector<Tile> group)
{
    // If the group is not 3/4 tiles
    if (group.size() < 3 || group.size() > 4)
    {
#if DEBUGMODE
        std::cout << "Group is wrong size!" << std::endl;
#endif
        return false;
    }
    Tile *lastTile = nullptr;
    for (size_t i = 0; i < group.size(); i++)
    {
        // If all the tiles do not have the same denomination
        if (lastTile && group[i].denomination != lastTile->denomination)
        {
#if DEBUGMODE
            std::cout << "A tile has the wrong denomination!" << std::endl;
#endif
            return false;
        }
        lastTile = &group[i];
    }

    for (size_t i = 0; i < group.size(); i++)
    {
        for (size_t j = i + 1; j < group.size(); j++)
        {
            // If any two tiles have the same color
            if (&group[i] != &group[j] && group[i].color == group[j].color)
            {
#if DEBUGMODE
                std::cout << "Two tiles have the same color!" << std::endl;
#endif
                return false;
            }
        }
    }

    return true;
}

void RummiKub::UndoAction(Action action, int index)
{
    // All of these do similar things
    // Put the tile back into the correct position in the hand so it stays sorted
    // Then remove it from whatever run or group it is in
    if (action.option == AddtoGroup)
    {

        hand.insert(hand.begin() + index, groups.at(action.num).back());
        groups.at(action.num).pop_back();
    }
    else if (action.option == AddtoRun)
    {
        hand.insert(hand.begin() + index, runs.at(action.num).back());
        runs.at(action.num).pop_back();
    }
    else if (action.option == MakenewGroup)
    {
        hand.insert(hand.begin() + index, groups.back().back());
        groups.pop_back();
    }
    else if (action.option == MakenewRun)
    {
        hand.insert(hand.begin() + index, runs.back().back());
        runs.pop_back();
    }
}

void RummiKub::Action::operator()(int index)
{
    // These all follow the same logic
    // Remove the correct tile from the hand
    // Push it onto the correct run or group
    // Or push a new run/group with this tile into the groups/runs vector
    if (option == AddtoRun)
    {
        hand.erase(hand.begin() + index);
        runs.at(num).push_back(tile);
    }
    else if (option == AddtoGroup)
    {
        hand.erase(hand.begin() + index);
        groups.at(num).push_back(tile);
    }
    else if (option == MakenewRun)
    {
        hand.erase(hand.begin() + index);
        std::vector<Tile> newRun;
        newRun.push_back(tile);
        runs.push_back(newRun);
    }
    else if (option == MakenewGroup)
    {
        hand.erase(hand.begin() + index);
        std::vector<Tile> newGroup;
        newGroup.push_back(tile);
        groups.push_back(newGroup);
    }
}

RummiKub::RummiKub()
{
    hand = std::vector<Tile>();
    groups = std::vector<std::vector<Tile>>();
    runs = std::vector<std::vector<Tile>>();
}

bool RummiKub::Solver(int index)
{
    // base case
    // out of tiles - check if runs and groups are legal
    // return true if yest and exit

    if (hand.empty() && AllLegal())
    {
        return true;
    }

    // Get all the options available to the current tile
    std::vector<Action> actions = GetOptions(index);

    // Run through each of these options
    for (size_t i = 0; i < actions.size(); i++)
    {
        iterations++;
// for debugging
// print current tile and table
#if DEBUGMODE
        std::cout << "\n=====================================================\n";
        std::cout << "hand: ";
        for (auto const &t : hand)
        {
            std::cout << t << " ";
        }
        std::cout << std::endl;
        std::cout << "table: current tile " << hand.at(index) << "\n";
        for (auto const &g : groups)
        {
            std::cout << "(G( ";
            for (auto const &t : g)
            {
                std::cout << t << " ";
            }
            std::cout << " )))\n";
        }
        for (auto const &r : runs)
        {
            std::cout << "(R( ";
            for (auto const &t : r)
            {
                std::cout << t << " ";
            }
            std::cout << " )))\n";
        }
#endif
        // Check if the current action is legal
        bool legal = ActionIsLegal(actions[i]);
        if (legal)
        {
#if DEBUGMODE
            std::cout << "Legal" << std::endl;
#endif
            // If it is legal then execute it
            actions[i](index);

// for debugging
// print current tile and table
#if DEBUGMODE
            std::cout << "\n=====================================================\n";
            std::cout << "hand: ";
            for (auto const &t : hand)
            {
                std::cout << t << " ";
            }
            std::cout << std::endl;
            for (auto const &g : groups)
            {
                std::cout << "(G( ";
                for (auto const &t : g)
                {
                    std::cout << t << " ";
                }
                std::cout << " )))\n";
            }
            for (auto const &r : runs)
            {
                std::cout << "(R( ";
                for (auto const &t : r)
                {
                    std::cout << t << " ";
                }
                std::cout << " )))\n";
            }
#endif
        }
        else
        {
#if DEBUGMODE
            std::cout << "Illegal" << std::endl;
#endif
        }

        // If the index is still valid and the hand is not empty then call recursively with the next tile
        //&& legal is very important, if the action is illegal then the program will eventually have to loop back anyway
        // so doing this massively increases efficiency, test 1 goes from 2.3 million iterations to about 6000
        if (index - 1 >= 0 && !hand.empty() && legal)
        {
            Solver(index - 1);
        }

        // If that action solved the rummikub
        if (hand.empty() && AllLegal())
        {
            return true;
        }

        // If the action was legal but was not correct for the solution then undo it
        if (legal)
        {
            UndoAction(actions[i], index);
        }
    }

    return false; // could not find place for current tile - backtrack
}

int RummiKub::CountDenomination(std::vector<Tile> countee, int denomination)
{
    int count = 0;
    for (size_t i = 0; i < countee.size(); i++)
    {
        // Counts all tiles within the given vector that have this denomination
        if (countee[i].denomination == denomination)
        {
            count++;
        }
    }
    return count;
}

int RummiKub::CountColorBefore(std::vector<Tile> countee, int index, Color color)
{
    int count = 0;

    for (int i = index - 1; i >= 0; i--)
    {
        // Counts all tiles before the given index in the vector which have this color
        if (countee[i].color == color)
        {
            count++;
        }
    }

    return count;
}