// ModularProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ModularNumbers.hpp"

int main()
{
    bool stillRunning = true;

	while (stillRunning)
	{
		int modeToRun = -1;

		std::cout << "Please input the mode you want to use. \n1 for shift cipher \n2 for smart shift cipher \n3 for Euclidean Algorithm \n4 for Chinese Algorithm" << std::endl;
		std::cin >> modeToRun;
		

		if (modeToRun == 1)
		{
			
				std::string input = "";
				std::cout << "Please input a string of all capital letters without spaces" << std::endl;
				std::cin >> input;
				

				for (int i = 0; i < 26; i++)
				{
					std::cout << i + 1 << " " << Decrypt(input, i) << std::endl;
				}

				int correctNum = 0;

				std::cout << "Please input the number of the string that looks correct" << std::endl;

				std::cin >> correctNum;
				

				std::cout << "The correct string is: " << Decrypt(input, correctNum - 1) << " which uses the decryption key: " << correctNum - 1 << std::endl;

			
		}
		else if (modeToRun == 2)
		{
			std::string input = "";
			std::cout << "Please input a string of all capital letters without spaces" << std::endl;
			std::cin >> input;


			std::vector<int> frequencies = FindFrequencies(input);

			for (int i = 0; i < 25; i++)
			{
				std::cout << "Frequency of " << static_cast<char>('A' + i) << " is: " << frequencies[i] << std::endl;
			}

			int mostFrequent = -1;
			int frequency = -1;
			for (int i = 0; i < 25; i++)
			{
				if (frequencies[i] > frequency)
				{
					frequency = frequencies[i];
					mostFrequent = i;
				}
			}

			std::cout << "The most frequent letter is: " << static_cast<char>('A' + mostFrequent) << std::endl;

			std::cout << "The decryption key is: " << std::abs(static_cast<int>('E') - (static_cast<int>('A') + mostFrequent)) << std::endl;

			std::cout << Decrypt(input, GetSmartShift(input)) << std::endl;

			//ZNGURZNGVPFNFNARKCERFFVBABSGURUHZNAZVAQERSYRPGFGURNPGVIRJVYYGURPBAGRZCYNGVIRERNFBANAQGURQRFVERSBENRFGURGVPCRESRPGVBAVGFONFVPRYRZRAGFNERYBTVPNAQVAGHVGVBANANYLFVFNAQPBAFGEHPGVBATRARENYVGLNAQVAQVIVQHNYVGL


		}
		else if (modeToRun == 3)
		{
			int a = 0;
			int b = 0;
			std::cout << "Please input a positive integer" << std::endl;
			std::cin >> a;

			std::cout << "Please input another positive integer" << std::endl;
			std::cin >> b;

			std::pair<int, int> solution = GetInverse(b, a);

			std::cout << "The greatest common divisor of " << a << " and " << b << " is " << solution.first << std::endl;
			std::cout << "The inverse modulo operator for these inputs is " << solution.second << std::endl;
		}
		else if (modeToRun == 4)
		{
			int k = 0;
			std::cout << "Please input the number of people" << std::endl;
			std::cin >> k;

			std::vector<std::pair<int, int>> pairs{};


			for (int i = 0; i < k; i++)
			{
				int first = 0;
				int second = 0;
				std::cout << "New Pair" << std::endl;
				std::cout << "Please input first value" << std::endl;
				std::cin >> first;
				std::cout << "Please input second value" << std::endl;
				std::cin >> second;

				pairs.push_back(std::make_pair(first, second));
			}

			int code = ChineseTheorem(pairs);

			std::cout << "The solution is: " << code << std::endl;
		}
		else
		{
			std::cout << "Error: " << modeToRun << " is not a valid mode, please try again" << std::endl;
			continue;
		}

		while (true)
		{

		char shouldRunAgain = '\0';

		std::cout << "Do you want to run again? y/n" << std::endl;
		std::cin >> shouldRunAgain;

		if (shouldRunAgain == 'y')
		{
			stillRunning = true;
			break;
		}
		else if (shouldRunAgain == 'n')
		{
			stillRunning = false;
			break;
		}
		else
		{
			std::cout << "Error: " << stillRunning << " is not a valid option, please try again" << std::endl;
			continue;
		}
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
