#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream>
#include <limits>

#define CRASHING 0
#define DEBUGMODE 0

bool Point::operator==(Point const &arg2) const
{
	return ((x == arg2.x) && (y == arg2.y));
}

std::ostream &operator<<(std::ostream &os, Point const &p)
{
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream &operator>>(std::istream &os, Point &p)
{
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// return value is (on left, on right)
std::pair<bool, bool> get_location(
	float const &p1x, // check which side of the line (p1x,p1y)-->(p2x,p2y)
	float const &p1y, // point (qx,qy) is on
	float const &p2x,
	float const &p2y,
	float const &qx,
	float const &qy)
{
	Point dir = {p2x - p1x, p2y - p1y};
	Point norm = {dir.y, -dir.x};
	Point point = {qx - p1x, qy - p1y};
	// scalar product is positive if on right side
	float scal_prod = norm.x * point.x + norm.y * point.y;
	return std::make_pair((scal_prod < 0), (scal_prod > 0));
}

// returns a set of indices of points that form convex hull
std::set<int> hullBruteForce(std::vector<Point> const &points)
{
	int num_points = points.size();
	// std::cout << "number of points " << num_points << std::endl;
	if (num_points < 3)
		throw "bad number of points";

	std::set<int> hull_indices;

	// First for loop for points
	for (int i = 0; i < num_points; i++)
	{
#if CRASHING
		std::cout << "Program crashed at i iteration: " << i << std::endl;
#endif
		// Second for loop for points
		for (int j = i + 1; j < num_points; j++)
		{
#if CRASHING
			std::cout << "Program crashed at j iteration: " << j << std::endl;
#endif
			// Make two pairs that we use to check whether all points are on 1 side of line
			std::pair<bool, bool> overall = std::make_pair(false, false);
			std::pair<bool, bool> temp = std::make_pair(false, false);

			// Third loop for points, these are the points that are being checked for what side of the line they're on
			for (int k = 0; k < num_points; k++)
			{
#if CRASHING
				std::cout << "Program crashed at k iteration: " << k << std::endl;
#endif
				// If the current point is either of the ones making the line then skip it
				if (points[k] == points[i] || points[k] == points[j])
				{
					continue;
				}

				// Get what side of the line this point is on
				temp = get_location(points[i].x, points[i].y, points[j].x, points[j].y, points[k].x, points[k].y);

#if DEBUGMODE
				if (temp.first)
				{
					std::cout << "Point: " << points[k] << " is left of line: " << points[i] << points[j] << std::endl;
				}
				else if (temp.second)
				{
					std::cout << "Point: " << points[k] << " is right of line: " << points[i] << points[j] << std::endl;
				}

#endif

				// If this is the first point on the left then set overall left to true
				if (overall.first == false && temp.first == true)
				{
					overall.first = true;
				}
				// If this is the first point on the right then set overall right to true
				if (overall.second == false && temp.second == true)
				{
					overall.second = true;
				}

				// If there are points on both sides of the line then just break out
				if (overall.first == overall.second)
				{
					break;
				}
			}

			// If there are only points on one side of the line
			if (overall.first != overall.second)
			{
#if DEBUGMODE
				std::cout << "Adding points: " << points[i] << " " << points[j] << std::endl;
#endif
				// Insert both points into the hull
				hull_indices.insert(i);
				hull_indices.insert(j);
			}
		}
	}
	return hull_indices;
}

std::vector<int> hullBruteForce2(std::vector<Point> const &points)
{
	int num_points = points.size();
	if (num_points < 3)
		throw "bad number of points";

	std::vector<int> hull_indices;

	// Keeps track of the current node being drawn from
	Point current = {0, 0};

	// For use in finding smallest node
	float lowestX = std::numeric_limits<float>::infinity();
	int lowIndex = 0;

#if CRASHING
	std::cout << "Crashing when finding lowest point" << std::endl;
#endif

	// This loop finds the leftmost node
	for (int i = 0; i < num_points; i++)
	{
		if (points[i].x < lowestX)
		{
			lowestX = points[i].x;
			current = points[i];
			lowIndex = i;

#if DEBUGMODE
			std::cout << "Lowest point is: " << points[i] << "at index: " << i << std::endl;
#endif
		}
	}

	// Put the first node into the vector
	hull_indices.push_back(lowIndex);

	// This loop goes through the points we are drawing the line to
	for (int i = 0; i < num_points; i++)
	{
#if CRASHING
		std::cout << "Crashing at i index: " << i << std::endl;
#endif
		// Same concept as other function, for use in keeping track of what side of the line points are on
		std::pair<bool, bool> overall = std::make_pair(false, false);
		std::pair<bool, bool> currentP = std::make_pair(false, false);

		// If this point is the one we're drawing from skip it
		if (points[i] == current)
		{
			continue;
		}

		// The points we're checking against the line
		for (int j = 0; j < num_points; j++)
		{
#if CRASHING
			std::cout << "Crashing at j index: " << j << std::endl;
#endif

			// If this point is either of the ones on the line skip it
			if (points[j] == current || points[j] == points[i])
			{
				continue;
			}

			// Get what side the current point is on
			currentP = get_location(current.x, current.y, points[i].x, points[i].y, points[j].x, points[j].y);

			// If this point is on the right then break out, we failed
			if (currentP.second == true && currentP.first == false)
			{
#if DEBUGMODE
				std::cout << "Point: " << points[j] << "was left of line: " << current << points[i] << std::endl;
#endif
				overall.second = true;
				break;
			}
		}

		// If there are no points on the right
		if (overall.second == false)
		{
			// If this point is the one we started from then break out, we've completed the hull
			if (i == hull_indices.front())
			{
#if DEBUGMODE
				std::cout << "Point: " << points[i] << "is the first point, algorithm complete" << std::endl;
#endif
				break;
			}

#if DEBUGMODE
			std::cout << "Adding point: " << points[i] << "to hull" << std::endl;
#endif

			// Set the new current to this point and push it to the vector
			current = points[i];
			hull_indices.push_back(i);

			// Reset the for loop
			i = -1;
		}
	}

	return hull_indices;
}
