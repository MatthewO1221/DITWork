// ProgrammingAssignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Logic.h"

int main()
{
	bool runAgain = true;

	while (runAgain)
	{
		bool valid = false;
		int numDice = 0;
		int sides = 0;
		int desiredSum = 0;

		int totalDoors = 0;
		int doorsToOpen = 0;
		int numTrials = 0;

		char mode;

		while (!valid)
		{
			std::cout << "Please select a mode: d for dice probability, m for monty hall ";
			std::cin >> mode;

			if (mode != 'd' && mode != 'm')
			{
				std::cout << "Error: that is not a valid mode, please try again" << std::endl;
				valid = false;
			}
			else
			{
				valid = true;
			}
		}

		if (mode == 'd')
		{
			valid = false;
			while (!valid)
			{
				std::cout << "Enter the number of dice: ";
				std::cin >> numDice;

				if (numDice < 1)
				{
					std::cout << "Error: that is not a valid number of dice, please try again" << std::endl;
					valid = false;
				}
				else
				{
					valid = true;
				}
			}

			valid = false;

			while (!valid)
			{
				std::cout << "Enter the number of sides: ";
				std::cin >> sides;

				if (sides < 2)
				{
					std::cout << "Error: that is not a valid number of sides, please try again" << std::endl;
					valid = false;
				}
				else
				{
					valid = true;
				}
			}

			valid = false;

			while (!valid)
			{
				std::cout << "Enter the desired sum: ";
				std::cin >> desiredSum;

				if (numDice > desiredSum || numDice * sides < desiredSum)
				{
					std::cout << "Error: that is not a valid sum, please try again" << std::endl;
					valid = false;
				}
				else
				{
					valid = true;
				}
			}

			valid = false;

			DiceProbability(numDice, sides, desiredSum);


			

		}
		else
		{
			valid = false;

			while (!valid)
			{
				std::cout << "Enter the total number of doors: ";
				std::cin >> totalDoors;

				if (totalDoors < 3)
				{
					std::cout << "Error: that is not a valid number of doors, please try again" << std::endl;
					valid = false;
				}
				else
				{
					valid = true;
				}
			}

			valid = false;

			while (!valid)
			{
				std::cout << "Enter the number of doors to be opened: ";
				std::cin >> doorsToOpen;

				if (doorsToOpen < 1 || doorsToOpen > totalDoors - 2)
				{
					std::cout << "Error: that is not a valid number of doors to open, please try again" << std::endl;
					valid = false;
				}
				else
				{
					valid = true;
				}
			}

			valid = false;

			while (!valid)
			{
				std::cout << "Enter the number of trials to run (more will give a more accurate result): ";
				std::cin >> numTrials;

				if (numTrials < 1)
				{
					std::cout << "Error: that is not a valid number of trials, please try again" << std::endl;
					valid = false;
				}
				else
				{
					valid = true;
				}
			}

			valid = false;

			MontyHall(totalDoors, doorsToOpen, numTrials);
		}

		while (!valid)
		{
			char run;
			std::cout << "Do you want to run again? y/n ";
			std::cin >> run;

			if (run == 'y')
			{
				runAgain = true;
				valid = true;
			}
			else if (run == 'n')
			{
				runAgain = false;
				valid = true;
			}
			else
			{
				std::cout << "Error: that is not a valid character, pelase try again" << std::endl;
				valid = false;
			}
		}
		
		
	}
}


