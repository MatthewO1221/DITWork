//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Rishabh Anand
// Project:		Project 0
// Course:		CS230S22
//
// Copyright � 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "Stub.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Vector2DZero(Vector2D* pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// This function sets the coordinates of the 2D vector (pResult) to x & y
void Vector2DSet(Vector2D* pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// In this function, pResult will be set to the opposite of pVec0 (e.g. x = -x & y = -y)
void Vector2DNeg(Vector2D* pResult, const Vector2D* pVec0)
{
	pResult->x = -1 * (pVec0->x);
	pResult->y = -1 * (pVec0->y);
}

// In this function, pResult will be the sum of pVec0 and pVec1 (x result = x0 + x1)
void Vector2DAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1)
{
	pResult->x = (pVec0->x) + (pVec1->x);
	pResult->y = (pVec0->y) + (pVec1->y);
}

// In this function, pResult will be the difference between pVec0 and pVec1 (specifically, pVec0 - pVec1)
void Vector2DSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1)
{
	pResult->x = (pVec0->x) - (pVec1->x);
	pResult->y = (pVec0->y) - (pVec1->y);
}

// In this function, pResult will be the unit vector of pVec0
// (NOTE: Care must be taken when pResult = pVec0!)
void Vector2DNormalize(Vector2D* pResult, const Vector2D* pVec0)
{	
	float length = Vector2DLength(pVec0);
	pResult->x = (pVec0->x / length);
	pResult->y = (pVec0->y / length);
}

// In this function, pResult will be the vector pVec0 scaled by the value 'scale'
void Vector2DScale(Vector2D* pResult, const Vector2D* pVec0, float scale)
{
	pResult->x = (pVec0->x * scale);
	pResult->y = (pVec0->y * scale);
}

// In this function, pResult will be the vector pVec0 scaled by 'scale' and added to pVec1
void Vector2DScaleAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1, float scale)
{
	pResult->x = ((pVec0->x * scale) + pVec1->x);
	pResult->y = ((pVec0->y * scale) + pVec1->y);
}

// In this function, pResult will be the vector pVec0 scaled by 'scale' and pVec1 will be subtracted from it
void Vector2DScaleSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1, float scale)
{
	pResult->x = ((pVec0->x * scale) - pVec1->x);
	pResult->y = ((pVec0->y * scale) - pVec1->y);
}

// This function returns the length of the vector pVec0
float Vector2DLength(const Vector2D* pVec0)
{
	return (sqrtf((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y)));
}

// This function returns the square of pVec0's length.
// NOTE: The square root function must NOT be called by this function.
float Vector2DSquareLength(const Vector2D* pVec0)
{
	return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
}

// This function returns to distance between two points.
float Vector2DDistance(const Vector2D* pVec0, const Vector2D* pVec1)
{
	float result;
	//Vector2D* temp = malloc(sizeof(Vector2D));
	Vector2D temp;
	Vector2DSub(&temp, pVec0, pVec1);
	result = Vector2DLength(&temp);
	return result;
}

// This function returns to distance between two points.
// NOTE: The square root function must NOT be called by this function.
float Vector2DSquareDistance(const Vector2D* pVec0, const Vector2D* pVec1)
{
	float result;
	//Vector2D* temp = malloc(sizeof(Vector2D));
	Vector2D temp;
	Vector2DSub(&temp, pVec0, pVec1);
	result = Vector2DSquareLength(&temp);
	return result;
}

// This function returns the dot product between pVec0 and pVec1
float Vector2DDotProduct(const Vector2D* pVec0, const Vector2D* pVec1)
{
	float xsquare = (pVec0->x * pVec1->x);
	float ysquare = (pVec0->y * pVec1->y);
	return (xsquare + ysquare);
}

// This function computes the coordinates of the unit vector represented by the angle "angle", which is in Degrees.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Vector2DFromAngleDeg(Vector2D* pResult, float angle)
{
	float radians = ((angle * (float)M_PI) / 180.0f);
	pResult->x = cosf(radians);
	pResult->y = sinf(radians);

}

// This function computes the coordinates of the unit vector represented by the angle "angle", which is in Radians.
// HINT: x = cos(angle), y = sin(angle).
void Vector2DFromAngleRad(Vector2D* pResult, float angle)
{
	float a = angle;
	pResult->x = cosf(a);
	pResult->y = sinf(a);
}

// This function computes the angle, in radians, of the specified vector.
// HINT: Angle (radians) = atan2f(direction Y, direction X).
// NOTE: If the pointer is NULL, then return 0.0f.
float Vector2DToAngleRad(const Vector2D* pVec)
{
	if (pVec != NULL)
	{
		return atan2f(pVec->y, pVec->x);
	}
	else
	{
		return 0.0f;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

