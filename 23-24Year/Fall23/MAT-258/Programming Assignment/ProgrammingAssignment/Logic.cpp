#include "Logic.h"




void DiceProbability(int numDice, int sides, int desiredSum)
{


	unsigned long long int totalOutcomes = pow(sides, numDice);
	int correctOutcomes = FindWays(numDice, sides, desiredSum);

	std::cout << "The number of outcomes with a sum of " << desiredSum << " is: " << correctOutcomes << std::endl;
	std::cout << "The probability of rolling the sum of " << desiredSum << " is: " << correctOutcomes << "/" << totalOutcomes << " or " <<
		(static_cast<double>(correctOutcomes) / totalOutcomes) * 100.0 << "%" << std::endl;
}

int FindWays(int numDice, int sides, int desiredSum)
{
	if (numDice == 0)
	{
		if (desiredSum == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int count = 0;

	for (int i = 1; i < sides + 1; i++)
	{
		if (desiredSum - i >= 0)
		{
			count += FindWays(numDice - 1, sides, desiredSum - i);
		}
	}

	return count;
}

void MontyHall(int numDoors, int doorsToOpen, int numTrials)
{
	
	int correct = 0;

	for (int i = 0; i < numTrials; i++)
	{
		std::vector<int> openedDoors;
		
		int carDoor = GetRandomDoor(0, numDoors - 1);

		int doorsOpened = 0;

		while (doorsOpened != doorsToOpen)
		{
			int door = GetRandomDoor(1, numDoors - 1);

			if (door == carDoor)
			{
				continue;
			}
			else
			{
				bool alreadyOpened = false;
				for (int j = 0; j < static_cast<int>(openedDoors.size()); j++)
				{
					if (openedDoors[j] == door)
					{
						alreadyOpened = true;
						break;
					}
				}

				if (!alreadyOpened)
				{
					openedDoors.push_back(door);
					doorsOpened++;
				}
			}
		}

		bool alreadyOpened = true;
		int newDoor;

		while (alreadyOpened)
		{
			newDoor = GetRandomDoor(1, numDoors - 1);

			for (int j = 0; j < static_cast<int>(openedDoors.size()); j++)
			{
				if (openedDoors[j] == newDoor)
				{
					alreadyOpened = true;
					break;
				}
				else
				{
					alreadyOpened = false;
				}
			}
		}
		
		if (newDoor == carDoor)
		{
			correct++;
		}

	}

	std::cout << "The player won the car " << correct << " times out of " << numTrials << ". Thus their estimated probability of winning with this many doors and opened doors is: "
		<< correct << "/" << numTrials << " or " << (static_cast<double>(correct)/numTrials) * 100.0 << "%" << std::endl;
}

int GetRandomDoor(int min, int max)
{
	//Use current time as seed
	std::mt19937 mt(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

	
	std::uniform_int_distribution<int> dist(min, max);

	
	return dist(mt);
}
