
#include <string>
#include <vector>
#include <tuple>

std::string Decrypt(std::string encrypted, int shiftBy);

std::vector<int> FindFrequencies(std::string encrypted);

int GetSmartShift(std::string encrypted);

std::pair<int, int> GetInverse(int a, int b);

int FindInverse(std::vector<std::tuple<int, int, int, int>> inverseValues, int callNum, std::vector<std::pair<bool, int>> *calculated = nullptr);

int ChineseTheorem(std::vector<std::pair<int, int>> pairs);
