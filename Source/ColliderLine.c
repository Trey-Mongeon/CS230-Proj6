//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderLine.h"

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


// Dynamically allocate a new (line) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
Collider* ColliderLineCreate(void)
{
	return NULL;
}


// Read the properties of a ColliderLine component from a file.
// (NOTE: First read an integer indicating the number of lines to be read.)
// (NOTE: For each line, read P0 and P1 using StreamReadVector2D.)
// (HINT: Call ColliderLineAddLineSegment() to add each line.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderLineRead(Collider* collider, Stream stream)
{
	UNREFERENCED_PARAMETER(collider);
	UNREFERENCED_PARAMETER(stream);
}


// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLineAddLineSegment(Collider* collider, const Vector2D* p0, const Vector2D* p1)
{
	UNREFERENCED_PARAMETER(collider);
	UNREFERENCED_PARAMETER(p0);
	UNREFERENCED_PARAMETER(p1);
}


// Check for collision between a line collider and a circle collider.
// Params:
//	 collider1 = Pointer to the line collider component.
//	 collider2 = Pointer to the circle collider component.
// Returns:
//	 If the pointers are valid,
//	   then return the results of a line-circle collision check,
//	   else return false.
bool ColliderLineIsCollidingWithCircle(const Collider* collider, const Collider* other)
{
	UNREFERENCED_PARAMETER(collider);
	UNREFERENCED_PARAMETER(other);
	return false;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

