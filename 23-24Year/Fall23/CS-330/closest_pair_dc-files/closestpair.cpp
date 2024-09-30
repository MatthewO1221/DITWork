/**
 * @file closestpair.cpp
 * @author Matthew Oliver
 * @brief
 * @version 0.1
 * @date 2023-10-25
 *
 *
 */

#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>
#include <memory>

#define DEBUG 0

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

/**
 * @brief Used when points do not all have the same X, find the closest pair of points from a given set
 *
 * @param points The set to find the closest pair in
 * @param size The size of the given set
 * @return float The distance between the closest points
 */
float closestPair_auxX(Point *points, const int size);

/**
 * @brief Used when points have the same X, find the closest pair of points from a given set
 *
 * @param points The set to find the closest pair in
 * @param size The size of the given set
 * @return float The distance between the closest points
 */
float closestPair_auxY(Point *points, const int size);

/**
 * @brief Used to sort the array by X values
 *
 * @param p1 The first point
 * @param p2 The second point
 * @return true Is the x of the first point smaller
 * @return false Is it larger or the same
 */
bool compareX(Point p1, Point p2)
{

	return p1.x < p2.x;
}

/**
 * @brief Used to sort the array by Y values
 *
 * @param p1 The first point
 * @param p2 The second point
 * @return true Is the y of the first point smaller
 * @return false Is it larger or the same
 */
bool compareY(Point p1, Point p2)
{

	return p1.y < p2.y;
}

/**
 * @brief Used to get the distance between 2 points
 *
 * @param p1 The first point
 * @param p2 The second point
 * @return float The distance between them
 */
float dist(Point p1, Point p2);

////////////////////////////////////////////////////////////////////////////////
float closestPair(std::vector<Point> const &points)
{
	const int size = points.size();
	float distance;

	if (size == 0)
		throw "zero size subset";
	if (size == 1)
		return std::numeric_limits<float>::max();
	if (size == 2)
	{
		return dist(points[0], points[1]);
	}

	// Make an array, this makes doing recursion easier
	// Point *newPoints = std::unique_ptr<Point>();
	std::unique_ptr<Point[]> newPointsUP(new Point[size]);

	Point *newPoints = newPointsUP.get();

	// Variables for determining vertical or all the same cases
	bool allTheSame = true;
	bool allTheSameX = true;

	// Put all of the points into the array, also allows us to check for special cases
	for (int i = 0; i < size; i++)
	{
		newPoints[i] = points[i];
		if (i > 1 && newPoints[i].x != newPoints[i - 1].x)
		{

			allTheSameX = false;
		}

		if (i > 1 && dist(newPoints[i], newPoints[i - 1]) != 0.0f)
		{
			allTheSame = false;
		}
	}

	// All points are the same so just return 0
	if (allTheSame)
	{

		return 0;
	}

	// All points have the same x so we should use a vertical solution
	if (allTheSameX)
	{
		// Sort array by y to make midpoint easier to find
		std::sort(newPoints, newPoints + size, compareY);

		distance = closestPair_auxY(newPoints, size);
	}
	else
	{

		std::sort(newPoints, newPoints + size, compareX);

		distance = closestPair_auxX(newPoints, size);
	}

	return distance;
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_auxY(Point *points, const int size)
{

	if (size == 0)
		return std::numeric_limits<float>::max();
	if (size == 1)
		return std::numeric_limits<float>::max();
	if (size == 2)
	{
		return dist(points[0], points[1]);
	}

	// Find the middle point, because array is sorted it is assumed to be at least close to even
	int middle = size / 2;
	Point midPoint = points[middle];

	// Call recursively
	float distanceLeft = closestPair_auxY(points, middle);
	float distanceRight = closestPair_auxY(points + middle, size - middle);

	// Distance is min of those two
	float distance = std::min(distanceLeft, distanceRight);

	// For each point on left side of midpoint
	for (int i = 0; i < middle; i++)
	{
		// If this point is within the minimum distance of the midpoint
		if (abs(points[i].y - midPoint.y) < distance)
		{
			// Check if its distance to the midpoint is closer
			if (dist(points[i], midPoint) < distance)
			{
				distance = dist(points[i], midPoint);
#if DEBUG
				std::cout << "Smaller distance found: " << distance << " using points: " << points[i] << midPoint << std::endl;
#endif
				i--;
				continue;
			}

			// For each point on right side of midpoint
			for (int j = middle + 1; j < size; j++)
			{
				// If this point is within the minimum distance of the midpoint
				if (abs(points[j].y - midPoint.y) < distance)
				{
					// Check if its distance to the midpoint is closer
					if (dist(points[j], midPoint) < distance)
					{
						distance = dist(points[j], midPoint);
#if DEBUG
						std::cout << "Smaller distance found: " << distance << " using points: " << points[j] << midPoint << std::endl;
#endif
						j--;
						continue;
					}

					// If these two points are closer than the current minimum distance
					if (dist(points[i], points[j]) < distance)
					{
						distance = dist(points[i], points[j]);
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	// Return the small distance found
	return distance;
}

float closestPair_auxX(Point *points, const int size)
{

	// SEE OTHER FUNCTION FOR COMMENTS, WORKS EXACTLY THE SAME WAY JUST COMPARING X INSTEAD OF Y

	if (size == 0)
		return std::numeric_limits<float>::max();
	if (size == 1)
		return std::numeric_limits<float>::max();
	if (size == 2)
	{
		return dist(points[0], points[1]);
	}

#if DEBUG
	std::cout << "Size: " << size << std::endl;
#endif

#if DEBUG
	for (int i = 0; i < size; i++)
	{
		std::cout << points[i];
	}

	std::cout << std::endl;

#endif
	int middle = size / 2;

#if DEBUG
	std::cout << "Middle: " << middle << std::endl;
#endif

	Point midPoint = points[middle];

#if DEBUG
	std::cout << midPoint << std::endl;
#endif

	float distanceLeft = closestPair_auxX(points, middle);
	float distanceRight = closestPair_auxX(points + middle, size - middle);

	float distance = std::min(distanceLeft, distanceRight);

#if DEBUG
	std::cout << "Distance: " << distance << std::endl;
#endif

	for (int i = 0; i < middle; i++)
	{

		if (abs(points[i].x - midPoint.x) < distance)
		{
			if (dist(points[i], midPoint) < distance)
			{
				distance = dist(points[i], midPoint);
#if DEBUG
				std::cout << "Smaller distance found: " << distance << " using points: " << points[i] << midPoint << std::endl;
#endif
				i--;
				continue;
			}

			for (int j = middle + 1; j < size; j++)
			{
				if (abs(points[j].x - midPoint.x) < distance)
				{
					if (dist(points[j], midPoint) < distance)
					{
						distance = dist(points[j], midPoint);
#if DEBUG
						std::cout << "Smaller distance found: " << distance << " using points: " << points[j] << midPoint << std::endl;
#endif
						j--;
						continue;
					}
					if (dist(points[i], points[j]) < distance)
					{
						distance = dist(points[i], points[j]);

#if DEBUG
						std::cout << "Smaller distance found: " << distance << " using points: " << points[i] << points[j] << std::endl;
#endif
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	return distance;
}

float dist(Point p1, Point p2)
{
	// Normal distance function
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
				(p1.y - p2.y) * (p1.y - p2.y));
}

/*
CLOSEST_PAIR( S set of points )
// recursion termination test:
if ( S.size() == 1 ) return infinity

divide S into two parts (Left and Right) with respect to a vertical line -- see Note 1
d1 = CLOSEST_PAIR(Left)
d2 = CLOSEST_PAIR(Right)
let d = min( d1, d2)
	for each point p1 in Left that are closer than d to the separating line {
		for each point p2 in Right that are closer than d to the separating line {
			if ( distance( p1,p2) < d ) {
				d = distance( p1,p2)
			}
		}
	}
return d;

Note 1: algorithm should take care of special case when vertical split cannot
separate the given set into 2 non-empty sets, like in
(1,0) (1,1) (1,2)
all 3 points are on one vertical line, so x-ordering (by vertical line) cannot
split the set. Switch to y-ordering (by horizontal line).  Notice that when
neither of the 2 ordering can split, you have a special case: all points in the
set are equal - thus distance is 0.
*/