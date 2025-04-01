//------------------------------------------------------------------------------
//
// File Name:	Random.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Random.h"
#include "Time.h"

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

// Initialize the pseudorandom number generator (PRNG).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
void RandomInit()
{
	srand((int)time(0));
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
	int result = rangeMin + rand() / (RAND_MAX /(rangeMax - rangeMin + 1) + 1);
	return result;
}

// Generate a random floating-point number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
float RandomRangeFloat(float rangeMin, float rangeMax)
{
	float result = (float)rand() / (RAND_MAX);
	float random = rangeMin + result * (rangeMax - rangeMin);
	return random;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

