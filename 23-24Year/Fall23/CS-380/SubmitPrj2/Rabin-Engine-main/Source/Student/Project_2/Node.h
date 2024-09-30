#pragma once
#include "Misc/PathfindingDetails.hpp"





enum class DirectionFlags : unsigned char
{
	North = 0b1,
	NorthEast = 0b10,
	East = 0b100,
	SouthEast = 0b1000,
	South = 0b10000,
	SouthWest = 0b100000,
	West = 0b1000000,
	NorthWest = 0b10000000,



	None = 0


};

enum class ListFlags : unsigned char
{
	Neither = 0b1,
	Open = 0b10,
	Closed = 0b100
};



DirectionFlags operator|(DirectionFlags lhs, DirectionFlags rhs);
DirectionFlags operator&(DirectionFlags lhs, DirectionFlags rhs);
DirectionFlags opposite(DirectionFlags dir);

ListFlags operator|(ListFlags lhs, ListFlags rhs);
ListFlags operator&(ListFlags lhs, ListFlags rhs);


class NodeLogic
{
public:
	NodeLogic();
	

	

	


	struct Node
	{
		Node();
		Node* GetNodeInDirection(DirectionFlags dir, NodeLogic *logic);
		void SetParent(Node* parent, DirectionFlags parentDir);
		float ComputeCost(Node* parent, DirectionFlags parentDir);
		float CalculateHeuristic(Heuristic use, GridPos goal, float weight);
		virtual void PrecomputeValidNeighbors(NodeLogic* logic);


		float givenCost;
		float heuristicCost;
		/**
		 * Which node is the parent node, first bit is North, then continues clockwise.
		 */
		DirectionFlags parent;
		/**
		 * Which list is this node on, first bit is neither, then open, then closed.
		 */
		ListFlags curList;
		/**
		 * Which neighbors are valid, first bit is North, then continues clockwise.
		 */
		DirectionFlags validNeighbors;

		unsigned char posRow;
		unsigned char posCol;

	};


	void Clear();

	
	void Precompute();

	 Node* GetNodeAtPos(GridPos pos);
	 Node* GetNodeAtPos(Vec3 pos);

	 void GetPath(Node* goal, Node* start);

	 
	 void RubberbandPath();
	 void SplinePath();
	 void RubberbandSplinePath();

	 std::vector<Vec3> path;
	Node nodes[40][40];
	Node* neighbors[40][40][8];



private:

};

