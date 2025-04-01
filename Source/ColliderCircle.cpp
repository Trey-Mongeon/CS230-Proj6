//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.cpp
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderCircle.h"
#include "Collider.h"
#include "Stream.h"
#include "Transform.h"
#include "DGL.h"
#include "Entity.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ColliderCircle
{
	// Inherit the base collider structure.
	Collider	base;

	// Radius of the circle collider.
	float radius;

} ColliderCircle;

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


// Dynamically allocate a new (circle) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
// (Note: Set the radius to a non-zero value.  Typical values are 0.5f or 1.0f.)
Collider* ColliderCircleCreate(void)
{
	ColliderCircle* newCollider = calloc(1, sizeof(ColliderCircle));

	if (newCollider)
	{
		newCollider->base.type = ColliderTypeCircle;
		newCollider->base.memorySize = sizeof(ColliderCircle);
		newCollider->radius = 1.0f;

		return (Collider*)newCollider;
	}
	else
	{
		return NULL;
	}
}

// Read the properties of a ColliderCircle component from a file.
// (NOTE: Read the radius value using StreamReadFloat.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderCircleRead(Collider* collider, Stream stream)
{
	if (collider)
	{
		ColliderCircle* circleCollider = (ColliderCircle*)collider;

		circleCollider->radius = StreamReadFloat(stream);
	}
}

// Get the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
// Returns:
//	 If the pointer is valid,
//	   then return the circle collider's radius,
//	   else return 0.0f.
float ColliderCircleGetRadius(const Collider* collider)
{
	if (collider)
	{
		ColliderCircle* circleCollider = (ColliderCircle*)collider;

		return circleCollider->radius;
	}
	else
	{
		return 0.0f;
	}
}

// Set the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
//   radius = the circle collider's new radius.
void ColliderCircleSetRadius(Collider* collider, float radius)
{
	if (collider)
	{
		ColliderCircle* circleCollider = (ColliderCircle*)collider;

		circleCollider->radius = radius;
	}
}

// Check for collision between two circle colliders.
// Params:
//	 collider1 = Pointer to the first circle collider component.
//	 collider2 = Pointer to the second circle collider component.
bool ColliderCircleIsCollidingWithCircle(const Collider* collider, const Collider* other)
{
	if (collider && other)
	{
		Transform* colliderTransform = EntityGetTransform(collider->parent);
		DGL_Vec2 colliderTranslation = *TransformGetTranslation(colliderTransform);

		Transform* otherTransform = EntityGetTransform(other->parent);
		DGL_Vec2 otherTranslation = *TransformGetTranslation(otherTransform);

		float lengthBetween = Vector2DDistance(&colliderTranslation, &otherTranslation);

		float radius1 = ColliderCircleGetRadius(collider);
		float radius2 = ColliderCircleGetRadius(collider);

		if (lengthBetween <= radius1 + radius2)
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

