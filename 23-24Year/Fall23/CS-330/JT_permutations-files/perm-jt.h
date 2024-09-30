#ifndef PERM_JOHNSON_TROTTER_H
#define PERM_JOHNSON_TROTTER_H
#include <vector>

class PermJohnsonTrotter
{

public:
	PermJohnsonTrotter(int size);
	bool Next();
	std::vector<int> const &Get() const;

private:
	enum Directions
	{
		Left = -1,
		Right = 1
	};

	std::vector<Directions> dir;
	std::vector<int> values;

	bool isMobile(int index);
	int findLargestMobile();
	void swapGreater(int value);
};
#endif
