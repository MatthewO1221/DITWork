/**
 * @file rummikub.h
 * @author Matthew Oliver
 * @brief
 * @version 0.1
 * @date 2023-02-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RUMMIKUB_H
#define RUMMIKUB_H
#include <fstream>
#include <vector>
#include <iostream>

/**
 * @brief Stores the colors that a rummikub can have
 *
 */
enum Color
{
    Red,
    Green,
    Blue,
    Yellow
};

/**
 * @brief This is a rummikub object
 *
 */
struct Tile
{
    int denomination;
    Color color;

    /**
     * @brief For use in std::sort
     *
     * @param other the other tile to compare denomination with
     * @return true If this tile is greater
     * @return false If this tile is smaller
     */
    bool operator>(const Tile &other) const
    {
        return denomination > other.denomination;
    }
};

inline std::ostream &operator<<(std::ostream &os, Tile const &t)
{
    os << "{ " << t.denomination << ",";
    switch (t.color)
    {
    case Red:
        os << "R";
        break;
    case Green:
        os << "G";
        break;
    case Blue:
        os << "B";
        break;
    case Yellow:
        os << "Y";
        break;
    }
    os << " }";
    return os;
}

/**
 * @brief This holds all the functions necessary to solve a given set of tiles
 *
 */
class RummiKub
{
public:
    /**
     * @brief Construct a new Rummi Kub object
     *
     */
    RummiKub(); // empty hand

    /**
     * @brief Adds a tile to the hand
     *
     */
    void Add(Tile const &); // add a tile to hand

    /**
     * @brief Sorts the tiles in the hand into valid runs and groups
     *
     */
    void Solve(); // solve

    // get solution - groups
    std::vector<std::vector<Tile>> GetGroups() const;
    // get solution - runs
    std::vector<std::vector<Tile>> GetRuns() const;
    // if both vectors are empty - no solution possible

private:
    /**
     * @brief Stores all the tiles in the hand
     *
     */
    static std::vector<Tile> hand;

    /**
     * @brief Stores the various groups, a group is a vector of tiles
     *
     */
    static std::vector<std::vector<Tile>> groups;

    /**
     * @brief Stores the various runs, a run is a vector of tiles
     *
     */
    static std::vector<std::vector<Tile>> runs;

    /**
     * @brief These are the different actions that a tile can take
     *
     */
    enum Options
    {
        AddtoGroup,
        AddtoRun,
        MakenewGroup,
        MakenewRun
    };

    /**
     * @brief This action object stores ALL the different actions a tile can take
     *
     */
    struct Action
    {
        /**
         * @brief Construct a new Action object
         *
         * @param tile The tile this action applies to
         * @param num The index of the group or run this action applies to
         * @param option What this action entails
         */
        Action(Tile tile, int num, Options option)
            : tile(tile), num(num), option(option)
        {
        }

        /**
         * @brief Construct a new Action object
         *
         * @param tile The tile this action applies to
         * @param option What this action entails
         */
        Action(Tile tile, Options option)
            : tile(tile), num(0), option(option)
        {
        }

        /**
         * @brief Executes the action
         *
         * @param index The index of the actionable tile within the hand
         */
        void operator()(int index);

        Tile tile;
        int num;
        Options option;
    };

    /**
     * @brief Creates a vector of all the possible actions a tile can take given the current board situation
     *
     * @param index The index of the current tile
     * @return std::vector<Action> All the actions the tile can take
     */
    std::vector<Action> GetOptions(int index); // Done

    /**
     * @brief Calls RunsLegal and GroupsLegal to check if everything is legal
     *
     * @return true If all groups and runs are legal
     * @return false If any groups or runs are illegal
     */
    bool AllLegal(); // Done

    /**
     * @brief Calls RunLegal on every run to check if they are all legal
     *
     * @return true If all runs are legal
     * @return false If any runs are illegal
     */
    bool RunsLegal(); // Done

    /**
     * @brief Calls GroupLegal on every group to check if they are all legal
     *
     * @return true If all groups are legal
     * @return false If any groups are illegal
     */
    bool GroupsLegal(); // Done

    /**
     * @brief Checks to see if the given run is legal
     *
     * @param run The run to check
     * @return true If the run is legal
     * @return false If the run is illegal
     */
    bool RunLegal(std::vector<Tile> run); // Done

    /**
     * @brief Checks to see if the given group is legal
     *
     * @param group The group to check
     * @return true If the group is legal
     * @return false If the group is illegal
     */
    bool GroupLegal(std::vector<Tile> group); // Done

    /**
     * @brief Checks to see if a given action is legal given the current board state
     *
     * @param action The action to check
     * @return true If the action is legal
     * @return false If the action is illegal
     */
    bool ActionIsLegal(Action action); // Done

    /**
     * @brief Undoes the given action, putting all tiles back where they were
     *
     * @param action The action to undo
     * @param index The index to put the undone tile back into
     */
    void UndoAction(Action action, int index); // Done

    /**
     * @brief The recursive function which actually solves the rummikub
     *
     * @param index The index of the current tile to run on
     * @return true If the solution has been found
     * @return false If the solution has not been found
     */
    bool Solver(int index);

    /**
     * @brief Counts the number of tiles with the given denomination in the given vector,
     * for use in seeing if creating or adding to a group is worth it
     *
     * @param countee The vector to check
     * @param denomination The denomination to check for
     * @return int The number of tiles with the given denomination
     */
    int CountDenomination(std::vector<Tile> countee, int denomination);

    /**
     * @brief Counts the number of tiles with the same color that occur before the given index in the given vector,
     * for use in seeing if creating or adding to a run is worth it
     *
     * @param countee The vector to check
     * @param index Check every tile before this index
     * @param color The color to check for
     * @return int The number of tiles of the given color before the given index
     */
    int CountColorBefore(std::vector<Tile> countee, int index, Color color);
};

#endif
