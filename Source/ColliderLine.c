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
#include "Vector2D.h"
#include "Stream.h"
#include "Collider.h"
#include "Entity.h"
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
#define cLineSegmentMax 32
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;


typedef struct ColliderLine
{
	// Inherit the base collider structure.
	Collider	base;

	// The number of line segments in the list.
	unsigned int		lineCount;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[cLineSegmentMax];

} ColliderLine;

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
	ColliderLine* newCollider = calloc(1, sizeof(ColliderLine));

	if (newCollider)
	{
		newCollider->base.type = ColliderTypeLine;
		newCollider->base.memorySize = sizeof(ColliderLine);

		return (Collider*)newCollider;
	}
	else
	{
		return NULL;
	}
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
	if (collider)
	{
		ColliderLine* lineCollider = (ColliderLine*)collider;

		int readAmt = StreamReadInt(stream);
		for (int i = 0; i < readAmt; ++i)
		{
			DGL_Vec2 p0;
			DGL_Vec2 p1;
			StreamReadVector2D(stream, &p0);
			StreamReadVector2D(stream, &p1);

			collider = (Collider*)lineCollider;
			ColliderLineAddLineSegment(collider, &p0, &p1);
		}
	}
}


// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLineAddLineSegment(Collider* collider, const Vector2D* p0, const Vector2D* p1)
{
	if (collider && p0 && p1)
	{
		ColliderLine* lineCollider = (ColliderLine*)collider;

		ColliderLineSegment segment = { 0 };

		segment.point[0] = *p0;
		segment.point[1] = *p1;

		lineCollider->lineSegments[lineCollider->lineCount] = segment;
		++lineCollider->lineCount;
	}
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
	if (collider && other)
	{
		Entity* entityO = other->parent;

		Physics* physicsO = EntityGetPhysics(entityO);
		Transform* transformO = EntityGetTransform(entityO);

		const DGL_Vec2 Pc = *TransformGetTranslation(transformO);
		const DGL_Vec2 Po = *PhysicsGetOldTranslation(physicsO);

		DGL_Vec2 dispO = { 0 };
		Vector2DSub(&dispO, &Pc, &Po);


		ColliderLine* lineCollider = (ColliderLine*)collider;

		for (unsigned int i = 0; i < lineCollider->lineCount; ++i)
		{
			ColliderLineSegment segment = lineCollider->lineSegments[i];

			DGL_Vec2 P0 = segment.point[0];
			DGL_Vec2 P1 = segment.point[1];

			DGL_Vec2 e = { 0 };
			Vector2DSub(&e, &P1, &P0);

			DGL_Vec2 n = { 0 };
			n.x = e.y;
			n.y = -e.x;

			Vector2DNormalize(&n, &n);

			if (Vector2DDotProduct(&dispO, &n) == 0)
			{
				continue;
			}

			if ((Vector2DDotProduct(&n, &Po) <= Vector2DDotProduct(&n, &P0)) && (Vector2DDotProduct(&n, &Pc) < Vector2DDotProduct(&n, &P0)))
			{
				continue;
			}

			if ((Vector2DDotProduct(&n, &Po) >= Vector2DDotProduct(&n, &P0)) && (Vector2DDotProduct(&n, &Pc) > Vector2DDotProduct(&n, &P0)))
			{
				continue;
			}

			float ti = (Vector2DDotProduct(&n, &P0) - Vector2DDotProduct(&n, &Po)) / Vector2DDotProduct(&n, &dispO);

			DGL_Vec2 Bi = { 0 };
			Vector2DScaleAdd(&Bi, &dispO, ti, &Po);


			DGL_Vec2 temp = { 0 };
			Vector2DSub(&temp, &P1, &P0);

			DGL_Vec2 temp1 = { 0 };
			Vector2DSub(&temp1, &Bi, &P0);

			if (Vector2DDotProduct(&temp, &temp1) < 0)
			{
				continue;
			}


			Vector2DSub(&temp, &P0, &P1);

			Vector2DSub(&temp1, &Bi, &P1);

			if (Vector2DDotProduct(&temp, &temp1) < 0)
			{
				continue;
			}

			DGL_Vec2 iVec = { 0 };
			Vector2DSub(&iVec, &Pc, &Bi);

			DGL_Vec2 s = { 0 };
			Vector2DScale(&s, &n, Vector2DDotProduct(&iVec, &n));

			DGL_Vec2 r = { 0 };
			DGL_Vec2 tempVec = { 0 };
			Vector2DScale(&tempVec, &s, 2.0f);
			Vector2DSub(&r, &iVec, &tempVec);

			DGL_Vec2 Br = { 0 };
			Vector2DAdd(&Br, &Bi, &r);

			float angle = Vector2DToAngleRad(&r);

			DGL_Vec2 finalVec = { 0 };
			Vector2DNormalize(&finalVec, &r);
			float speed = Vector2DLength(PhysicsGetVelocity(physicsO));
			Vector2DScale(&finalVec, &finalVec, speed);
			
			PhysicsSetVelocity(physicsO, &finalVec);
			TransformSetTranslation(transformO, &Br);
			TransformSetRotation(transformO, angle);

			continue;
		}



		return false;
	}
	else
	{
		return false;
	}
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

