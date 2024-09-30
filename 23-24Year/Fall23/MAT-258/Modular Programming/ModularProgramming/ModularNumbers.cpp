
#include "ModularNumbers.hpp"
#include <math.h>

static const int NUM_LETTERS = 26;


std::string Decrypt(std::string encrypted, int shiftBy)
{

	std::string decrypted = encrypted;


	for (char& character : decrypted)
	{
		
			character = 'A' + (character - 'A' + shiftBy) % NUM_LETTERS;
		
		
	}


	return decrypted;
	
}

std::vector<int> FindFrequencies(std::string encrypted)
{
	std::vector<int> frequencies{};
	frequencies.resize(26);

	for (auto character : encrypted)
	{
		frequencies[static_cast<int>(character - 'A')]++;
	}
	
	
	return frequencies;
}

int GetSmartShift(std::string encrypted)
{
	std::vector<int> frequencies = FindFrequencies(encrypted);

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

	return std::abs(static_cast<int>('E') - (static_cast<int>('A') + mostFrequent));
}


std::pair<int, int> GetInverse(int a, int b)
{
	std::vector<std::tuple<int, int, int, int>> inverseValues{};

	int x = a;
	int y = b;
	int z = -1;
	
	int i = 0;

	while (z != 0)
	{
		z = x % y;

		inverseValues.push_back(std::make_tuple(z, x, y, -((x - z) / y)));

		x = y;
		y = z;

		i++;
	}

	if (std::get<0>(inverseValues[i - 2]) == 1)
	{
		return std::make_pair(std::get<0>(inverseValues[i - 2]), FindInverse(inverseValues, 0));
	}
	else
	{
		return std::make_pair(std::get<0>(inverseValues[i - 2]), -1);
	}
}

int FindInverse(std::vector<std::tuple<int, int, int, int>> inverseValues, int callNum, std::vector<std::pair<bool, int>> *calculated)
{
	
	if (callNum == 0)
	{
		calculated = new std::vector<std::pair<bool, int>>();

		for (int i = 0; i < static_cast<int>(inverseValues.size()); i++)
		{
			calculated->push_back(std::make_pair(false, 0));
		}

		calculated->push_back(std::make_pair(true, 0));
		calculated->at(static_cast<int>(inverseValues.size()) - 1).first = true;
		calculated->at(static_cast<int>(inverseValues.size()) - 1).second = 1;

		int invplus1 = 0;
		int invplus2 = 0;
		if (calculated->at(callNum + 1).first)
		{
			invplus1 = calculated->at(callNum + 1).second;
		}
		else
		{
			invplus1 = FindInverse(inverseValues, callNum + 1, calculated);
			calculated->at(callNum + 1).first = true;
			calculated->at(callNum + 1).second = invplus1;
		}

		if (calculated->at(callNum + 2).first)
		{
			invplus2 = calculated->at(callNum + 2).second;
		}
		else
		{
			invplus2 = FindInverse(inverseValues, callNum + 2, calculated);
			calculated->at(callNum + 2).first = true;
			calculated->at(callNum + 2).second = invplus2;
		}
		
		int quotient = std::get<3>(inverseValues[callNum]);
		int modulo = std::get<1>(inverseValues[callNum]);

		delete calculated;
		
		int finalInv = (invplus1 * quotient) + invplus2;
		int fixedInv = (finalInv % modulo + modulo) % modulo;
		return fixedInv;
	}
	else
	{
		int invplus1 = 0;
		int invplus2 = 0;
		if (calculated->at(callNum + 1).first)
		{
			invplus1 = calculated->at(callNum + 1).second;
		}
		else
		{
			invplus1 = FindInverse(inverseValues, callNum + 1, calculated);
			calculated->at(callNum + 1).first = true;
			calculated->at(callNum + 1).second = invplus1;
		}

		if (calculated->at(callNum + 2).first)
		{
			invplus2 = calculated->at(callNum + 2).second;
		}
		else
		{
			invplus2 = FindInverse(inverseValues, callNum + 2, calculated);
			calculated->at(callNum + 2).first = true;
			calculated->at(callNum + 2).second = invplus2;
		}

		int quotient = std::get<3>(inverseValues[callNum]);
		
		
		return (invplus1 * quotient) + invplus2;
	}
}

int ChineseTheorem(std::vector<std::pair<int, int>> pairs)
{
	int product = 1;
	for (const auto& pair : pairs) 
	{
		product *= pair.first;
	}

	int result = 0;
	for (const auto& pair : pairs) 
	{
		int divisor = pair.first;
		int remainder = pair.second;
		int partialProduct = product / divisor;
		int inv = GetInverse(partialProduct, divisor).second;
		result += remainder * partialProduct * inv;
	}

	return (result % product + product) % product;
}
