#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "Node.h"


static const float sqrt2 = 1.41421356237f;

NodeLogic::NodeLogic()
{
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			nodes[i][j] = Node();
			nodes[i][j].posRow = static_cast<unsigned char>(i);
			nodes[i][j].posCol = static_cast<unsigned char>(j);

			for (int k = 0; k < 8; k++)
			{
				neighbors[i][j][k] = nullptr;
			}
		}
	}
}

void NodeLogic::Clear()
{
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			nodes[i][j].curList = ListFlags::Neither;
			nodes[i][j].heuristicCost = 0.0f;
		}
	}
}


NodeLogic::Node* NodeLogic::GetNodeAtPos(GridPos pos)
{
	return &nodes[pos.row][pos.col];
}

NodeLogic::Node* NodeLogic::GetNodeAtPos(Vec3 pos)
{
	GridPos gridPos = terrain->get_grid_position(pos);
	return &nodes[gridPos.row][gridPos.col];
}


void NodeLogic::Precompute()
{
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			nodes[i][j].validNeighbors = DirectionFlags::None;
			nodes[i][j].PrecomputeValidNeighbors(this);
			
		}
	}
}

void NodeLogic::GetPath(NodeLogic::Node* goal, NodeLogic::Node* start)
{
	if (goal != start)
	{
		GetPath(goal->GetNodeInDirection(goal->parent, this), start);
	}

	path.push_back(terrain->get_world_position(GridPos{ static_cast<int>(goal->posRow), static_cast<int>(goal->posCol) }));
}

//////////////////////////////////////////Node Code////////////////////////////////////////////////////////


NodeLogic::Node::Node()
	: parent(DirectionFlags::None), curList(ListFlags::Neither), validNeighbors(DirectionFlags::None), posRow(0), posCol(0)
{

}

NodeLogic::Node* NodeLogic::Node::GetNodeInDirection(DirectionFlags dir, NodeLogic *logic)
{
	int dirInt = static_cast<int>(dir);
	int i = 0;
	for (i; dirInt > 1; i++)
	{
		dirInt >>= 1;
	}
	return logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][i];
}


void NodeLogic::Node::SetParent(NodeLogic::Node* parent, DirectionFlags parentDir)
{
	this->parent = opposite(parentDir);
	this->givenCost = ComputeCost(parent, parentDir);
}


float NodeLogic::Node::ComputeCost(NodeLogic::Node* parent, DirectionFlags parentDir)
{
	if (static_cast<bool>(parentDir & DirectionFlags::North) 
		|| static_cast<bool>(parentDir & DirectionFlags::East) 
		|| static_cast<bool>(parentDir & DirectionFlags::South) 
		|| static_cast<bool>(parentDir & DirectionFlags::West))
	{
		return parent->givenCost + 1.0f;
	}
	else
	{
		return parent->givenCost + sqrt2;
	}
}

float NodeLogic::Node::CalculateHeuristic(Heuristic use, GridPos goal, float weight)
{
	int rowDiff = abs(goal.row - static_cast<int>(posRow));
	int colDiff = abs(goal.col - static_cast<int>(posCol));
	float mini = static_cast<float>(std::min(rowDiff, colDiff));
	switch (use)
	{
	case Heuristic::OCTILE:
		

		return weight * ((mini *
			sqrt2) +
			static_cast<float>(std::max(rowDiff, colDiff)) -
			mini);
		break;
	case Heuristic::CHEBYSHEV:
		return weight * (static_cast<float>(std::max(abs(goal.row - static_cast<int>(posRow)), abs(goal.col - static_cast<int>(posCol)))));
		break;
	case Heuristic::INCONSISTENT:
		if ((static_cast<int>(posRow) + static_cast<int>(posCol)) % 2 > 0)
		{
			return weight * (sqrtf((powf(static_cast<float>(abs(goal.row - static_cast<int>(posRow))), 2.0f) + powf(static_cast<float>(abs(goal.col - static_cast<int>(posCol))), 2.0f))));
		}
		
		return 0.0f;
		break;
	case Heuristic::MANHATTAN:
		return weight * (static_cast<float>((abs(goal.row - static_cast<int>(posRow)) + abs(goal.col - static_cast<int>(posCol)))));
		break;
	case Heuristic::EUCLIDEAN:
		return weight * (sqrtf((powf(static_cast<float>(abs(goal.row - static_cast<int>(posRow))), 2.0f) + powf(static_cast<float>(abs(goal.col - static_cast<int>(posCol))), 2.0f))));
		break;
	default:
		return 0.0f;
		break;
	}
}

void NodeLogic::Node::PrecomputeValidNeighbors(NodeLogic* logic)
{
	for (int i = 1; i < 129; i *=2)
	{
		switch (static_cast<DirectionFlags>(i))
		{
		case DirectionFlags::North:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow) + 1, static_cast<int>(posCol))
				&& !terrain->is_wall(static_cast<int>(posRow) + 1, static_cast<int>(posCol)))
			{
				validNeighbors = validNeighbors | DirectionFlags::North;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][0] = &logic->nodes[static_cast<int>(posRow) + 1][static_cast<int>(posCol)];
			}
			break;
		case DirectionFlags::East:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow), static_cast<int>(posCol) + 1)
				&& !terrain->is_wall(static_cast<int>(posRow), static_cast<int>(posCol) + 1))
			{
				validNeighbors = validNeighbors | DirectionFlags::East;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][2] = &logic->nodes[static_cast<int>(posRow)][static_cast<int>(posCol) + 1];
			}
			break;
		case DirectionFlags::South:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow) - 1, static_cast<int>(posCol))
				&& !terrain->is_wall(static_cast<int>(posRow) - 1, static_cast<int>(posCol)))
			{
				validNeighbors = validNeighbors | DirectionFlags::South;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][4] = &logic->nodes[static_cast<int>(posRow) - 1][static_cast<int>(posCol)];
			}
			break;
		case DirectionFlags::West:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow), static_cast<int>(posCol) - 1)
				&& !terrain->is_wall(static_cast<int>(posRow), static_cast<int>(posCol) - 1))
			{
				validNeighbors = validNeighbors | DirectionFlags::West;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][6] = &logic->nodes[static_cast<int>(posRow)][static_cast<int>(posCol) - 1];
			}
			break;
		case DirectionFlags::NorthEast:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow) + 1, static_cast<int>(posCol) + 1)
				&& !terrain->is_wall(static_cast<int>(posRow) + 1, static_cast<int>(posCol) + 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow), static_cast<int>(posCol) + 1)
				&& !terrain->is_wall(static_cast<int>(posRow), static_cast<int>(posCol) + 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow) + 1, static_cast<int>(posCol))
				&& !terrain->is_wall(static_cast<int>(posRow) + 1, static_cast<int>(posCol)))
			{
				validNeighbors = validNeighbors | DirectionFlags::NorthEast;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][1] = &logic->nodes[static_cast<int>(posRow) + 1][static_cast<int>(posCol) + 1];
			}
			break;
		case DirectionFlags::SouthEast:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow) - 1, static_cast<int>(posCol) + 1)
				&& !terrain->is_wall(static_cast<int>(posRow) - 1, static_cast<int>(posCol) + 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow), static_cast<int>(posCol) + 1)
				&& !terrain->is_wall(static_cast<int>(posRow), static_cast<int>(posCol) + 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow) - 1, static_cast<int>(posCol))
				&& !terrain->is_wall(static_cast<int>(posRow) - 1, static_cast<int>(posCol)))
			{
				validNeighbors = validNeighbors | DirectionFlags::SouthEast;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][3] = &logic->nodes[static_cast<int>(posRow) - 1][static_cast<int>(posCol) + 1];
			}
			break;
		case DirectionFlags::SouthWest:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow) - 1, static_cast<int>(posCol) - 1)
				&& !terrain->is_wall(static_cast<int>(posRow) - 1, static_cast<int>(posCol) - 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow), static_cast<int>(posCol) - 1)
				&& !terrain->is_wall(static_cast<int>(posRow), static_cast<int>(posCol) - 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow) - 1, static_cast<int>(posCol))
				&& !terrain->is_wall(static_cast<int>(posRow) - 1, static_cast<int>(posCol)))
			{
				validNeighbors = validNeighbors | DirectionFlags::SouthWest;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][5] = &logic->nodes[static_cast<int>(posRow) - 1][static_cast<int>(posCol) - 1];
			}
			break;
		case DirectionFlags::NorthWest:
			if (terrain->is_valid_grid_position(static_cast<int>(posRow) + 1, static_cast<int>(posCol) - 1)
				&& !terrain->is_wall(static_cast<int>(posRow) + 1, static_cast<int>(posCol) - 1)
				&& terrain->is_valid_grid_position(static_cast<int>(posRow) + 1, static_cast<int>(posCol))
				&& !terrain->is_wall(static_cast<int>(posRow) + 1, static_cast<int>(posCol))
				&& terrain->is_valid_grid_position(static_cast<int>(posRow), static_cast<int>(posCol) - 1)
				&& !terrain->is_wall(static_cast<int>(posRow), static_cast<int>(posCol) - 1))
			{
				validNeighbors = validNeighbors | DirectionFlags::NorthWest;
				logic->neighbors[static_cast<int>(posRow)][static_cast<int>(posCol)][7] = &logic->nodes[static_cast<int>(posRow) + 1][static_cast<int>(posCol) - 1];
			}
			break;
		default:
			break;
		}
	}
}




////////////////////////////////////////Direction Flags Code///////////////////////////////////////////////


DirectionFlags operator|(DirectionFlags lhs, DirectionFlags rhs)
{
	using DirectionFlagsType = std::underlying_type<DirectionFlags>::type;
	return DirectionFlags(static_cast<DirectionFlagsType>(lhs) | static_cast<DirectionFlagsType>(rhs));
}

DirectionFlags operator&(DirectionFlags lhs, DirectionFlags rhs) 
{
	using DirectionFlagsType = std::underlying_type<DirectionFlags>::type;
	return DirectionFlags(static_cast<DirectionFlagsType>(lhs) & static_cast<DirectionFlagsType>(rhs));
}

DirectionFlags opposite(DirectionFlags dir)
{
	switch (dir)
	{
	case DirectionFlags::North:
		return DirectionFlags::South;
		break;
	case DirectionFlags::NorthEast:
		return DirectionFlags::SouthWest;
		break;
	case DirectionFlags::East:
		return DirectionFlags::West;
		break;
	case DirectionFlags::SouthEast:
		return DirectionFlags::NorthWest;
		break;
	case DirectionFlags::South:
		return DirectionFlags::North;
		break;
	case DirectionFlags::SouthWest:
		return DirectionFlags::NorthEast;
		break;
	case DirectionFlags::West:
		return DirectionFlags::East;
		break;
	case DirectionFlags::NorthWest:
		return DirectionFlags::SouthEast;
		break;
	default:
		return DirectionFlags::None;
		break;
	}
}

//////////////////////////////////////List Flags Code///////////////////////////////////////////////////////////



ListFlags operator|(ListFlags lhs, ListFlags rhs)
{
	using ListFlagsType = std::underlying_type<ListFlags>::type;
	return ListFlags(static_cast<ListFlagsType>(lhs) | static_cast<ListFlagsType>(rhs));
}

ListFlags operator&(ListFlags lhs, ListFlags rhs) 
{
	using ListFlagsType = std::underlying_type<ListFlags>::type;
	return ListFlags(static_cast<ListFlagsType>(lhs) & static_cast<ListFlagsType>(rhs));
}




//////////////////////////////////////Fancy Paths////////////////////////////////////////////////////////////////


void NodeLogic::RubberbandPath()
{
	for (int i = 0; i + 2 < static_cast<int>(path.size()); i++)
	{
		GridPos firstPoint = terrain->get_grid_position(path[i]);
		GridPos secondPoint = terrain->get_grid_position(path[i + 2]);

		int lowRow = std::min(firstPoint.row, secondPoint.row);
		int highRow = std::max(firstPoint.row, secondPoint.row);

		int lowCol = std::min(firstPoint.col, secondPoint.col);
		int highCol = std::max(firstPoint.col, secondPoint.col);

		bool allValid = true;

		for (int j = lowRow; j <= highRow; j++)
		{
			for (int k = lowCol; k <= highCol; k++)
			{
				if (!terrain->is_valid_grid_position(j, k)
					|| terrain->is_wall(j, k))
				{
					allValid = false;
				}
			}
		}

		if (allValid)
		{
			path.erase(path.begin() + i + 1); 
			i--;
		}

	}
}


void NodeLogic::SplinePath()
{
	std::vector<Vec3> newPath;

	if (path.size() <= 2)
	{
		return;
	}
	else
	{
		for (int i = 0; i < static_cast<int>(path.size()) - 1; i++)
		{
			if (i == 0)
			{
				newPath.push_back(path[0]);
				newPath.push_back(Vec3::CatmullRom(path[0], path[0], path[1], path[2], 0.25f));
				newPath.push_back(Vec3::CatmullRom(path[0], path[0], path[1], path[2], 0.5f));
				newPath.push_back(Vec3::CatmullRom(path[0], path[0], path[1], path[2], 0.75f));
			}
			else if (i == path.size() - 2)
			{
				newPath.push_back(path[i]);
				newPath.push_back(Vec3::CatmullRom(path[i - 1], path[i], path[i + 1], path[i + 1], 0.25f));
				newPath.push_back(Vec3::CatmullRom(path[i - 1], path[i], path[i + 1], path[i + 1], 0.5f));
				newPath.push_back(Vec3::CatmullRom(path[i - 1], path[i], path[i + 1], path[i + 1], 0.75f));

			}
			else
			{
				newPath.push_back(path[i]);
				newPath.push_back(Vec3::CatmullRom(path[i - 1], path[i], path[i + 1], path[i + 2], 0.25f));
				newPath.push_back(Vec3::CatmullRom(path[i - 1], path[i], path[i + 1], path[i + 2], 0.5f));
				newPath.push_back(Vec3::CatmullRom(path[i - 1], path[i], path[i + 1], path[i + 2], 0.75f));
			}
		}

		newPath.push_back(path[path.size() - 1]);

		path = newPath;
	}

	
}


void NodeLogic::RubberbandSplinePath()
{
	RubberbandPath();

	for (int i = 0; i < static_cast<int>(path.size()) - 1; i++)
	{
		GridPos firstPos = terrain->get_grid_position(path[i]);
		GridPos secondPos = terrain->get_grid_position(path[i + 1]);

		if (powf(fabsf(static_cast<float>(firstPos.row) - static_cast<float>(secondPos.row)), 2.0f) 
			+ powf(fabsf(static_cast<float>(firstPos.col) - static_cast<float>(secondPos.col)), 2.0f) > 2.25f)
		{
			Vec3 midPoint = {(path[i].x + path[i + 1].x) / 2.0f,
			(path[i].y + path[i + 1].y) / 2.0f ,
			(path[i].z + path[i + 1].z) / 2.0f };
			path.insert(path.begin() + i + 1, midPoint);

			i--;
		}
	}

	SplinePath();
}
