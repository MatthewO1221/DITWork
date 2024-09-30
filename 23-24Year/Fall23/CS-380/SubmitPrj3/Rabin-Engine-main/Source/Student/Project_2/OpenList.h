#pragma once
#include "Node.h"


static const float bucketSize = 0.1f;
static const int numBuckets = 2000;


class OpenList
{
public:
	
	OpenList();
	

	void Push(NodeLogic::Node* node, Heuristic use, GridPos goal, float weight);
	NodeLogic::Node* Pop();
	void UpdateLowest();
	void Clear();
	void Remove(NodeLogic::Node* node, Heuristic use, GridPos goal, float weight);

	std::vector<std::pair<float, NodeLogic::Node*>>* list[numBuckets];
	int lowIndex;
	float costSpace;
	bool listEmpty;
private:

};

