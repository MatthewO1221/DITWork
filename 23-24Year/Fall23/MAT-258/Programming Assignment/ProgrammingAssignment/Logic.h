#include <iostream>
#include <random>
#include <math.h>
#include <chrono>


/**
 * Calculates the probability that a given number of dice, each with a given number of sides, add up to a desired sum.
 */
void DiceProbability(int numDice, int sides, int desiredSum);

/**
 * Recursively finds the number of ways to achieve a given sum with dice.
 */
int FindWays(int numDice, int sides, int desiredSum);

void MontyHall(int numDoors, int doorsToOpen, int numTrials);

int GetRandomDoor(int min, int max);
