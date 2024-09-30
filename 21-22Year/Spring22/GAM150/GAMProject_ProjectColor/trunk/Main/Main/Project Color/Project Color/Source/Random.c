//------------------------------------------------------------------------------
//
// File Name:	Random.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
#include "stdafx.h"
#include "Random.h"
#include <stdlib.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the random number generator (RNG).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
void RandomInit()
{
	//Used info from https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand
	srand(0);
}

// Generate a random number in the interval [rangeMin, rangeMax] (inclusive).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
// Returns:
//   A random number in the interval [rangeMin, rangeMax].
int RandomRange(int rangeMin, int rangeMax)
{
	//Function taken from https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand
	return rand() % (rangeMax - rangeMin + 1) + rangeMin;
}

// Generate a random floating-point number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
float RandomRangeFloat(float rangeMin, float rangeMax)
{
	//Function taken from https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand
	float r = ((float)rand() / RAND_MAX) * (rangeMax - rangeMin) + rangeMin;
	return r;
}

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

