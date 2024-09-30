#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "OpenList.h"
#include <limits>



OpenList::OpenList()
	:list(), lowIndex(numBuckets - 1), costSpace(bucketSize), listEmpty(true)
{
	for (int i = 0; i < numBuckets; i++)
	{
		list[i] = new std::vector<std::pair<float, NodeLogic::Node*>>;
		list[i]->reserve(50);
	}
}



void OpenList::Push(NodeLogic::Node* node, Heuristic use, GridPos goal, float weight)
{
	
	if (node->heuristicCost == 0.0f)
	{
		node->heuristicCost = node->CalculateHeuristic(use, goal, weight);
	}
	
	float totalCost = node->givenCost + node->heuristicCost;

	
	int index = static_cast<int>(totalCost / costSpace);

	if (index < lowIndex)
	{
		lowIndex = index;
	}

	listEmpty = false;
	list[index]->push_back(std::pair<float, NodeLogic::Node*>(totalCost, node));
	node->curList = ListFlags::Open;
}


NodeLogic::Node* OpenList::Pop()
{
	float lowestCost = std::numeric_limits<float>::max();
	int lowestIndex = 0;
	NodeLogic::Node* returnNode = nullptr;
	for (int i = 0; i < static_cast<int>(list[lowIndex]->size()); i++)
	{
		if (list[lowIndex]->at(i).first < lowestCost)
		{
			lowestCost = list[lowIndex]->at(i).first;
			returnNode = list[lowIndex]->at(i).second;
			lowestIndex = i;
		}
	}

	list[lowIndex]->erase(list[lowIndex]->begin() + lowestIndex);

	if (list[lowIndex]->empty())
	{
		UpdateLowest();
	}

	returnNode->curList = ListFlags::Closed;

	return returnNode;
}

void OpenList::UpdateLowest()
{
	bool found = false;
	for (int i = lowIndex; i < numBuckets; i++)
	{
		if (!list[i]->empty())
		{
			lowIndex = i;
			found = true;
			break;
		}
	}

	if (!found)
	{
		lowIndex = numBuckets - 1;
		listEmpty = true;
	}
}

void OpenList::Clear()
{
	for (int i = lowIndex; i < numBuckets; i++)
	{
		list[i]->clear();
	}

	lowIndex = numBuckets - 1;
	listEmpty = true;
}


void OpenList::Remove(NodeLogic::Node* node, Heuristic use, GridPos goal, float weight)
{
	float totalCost = node->givenCost + node->heuristicCost;
	int index = static_cast<int>(totalCost / costSpace);

	for (int j = 0; j  < static_cast<int>(list[index]->size()); j ++)
	{
		if (list[index]->at(j).second == node)
		{
			list[index]->erase(list[index]->begin() + j);
		}
	}
		
}
