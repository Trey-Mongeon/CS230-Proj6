//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"
#include "Transform.h"
#include "Entity.h"
#include "DGL.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Collider
{
	// Pointer to the collider's parent Entity.
	Entity* parent;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler	handler;

} Collider;

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


// Dynamically allocate a new Collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Collider* ColliderCreate(void)
{
	Collider* colliderPtr = calloc(1, sizeof(Collider));

	if (colliderPtr)
	{
		return colliderPtr;
	}
	else
	{
		return NULL;
	}
}


// Dynamically allocate a clone of an existing Collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Collider* ColliderClone(const Collider* other)
{
	if (other)
	{
		Collider* newCollider = calloc(1, sizeof(Collider));

		if (newCollider)
		{
			*newCollider = *other;
			return newCollider;
		}
	}
	return NULL;
}


// Free the memory associated with a Collider component.
// (Also, set the collider pointer to NULL.)
// Params:
//	 collider = Pointer to the Collider component.
void ColliderFree(Collider** collider)
{
	if (collider && *collider)
	{
		free(*collider);
		*collider = NULL;
	}
}


// Read the properties of a Collider component from a file.
// [NOTE: No values need to be read at this time.]
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderRead(Collider* collider, Stream stream)
{
	UNREFERENCED_PARAMETER(collider);
	UNREFERENCED_PARAMETER(stream);
}


// Set the parent Entity for a Collider component.
// Params:
//	 collider = Pointer to the Collider component.
//	 parent = Pointer to the parent Entity.
void ColliderSetParent(Collider* collider, Entity* parent)
{
	if (collider && parent)
	{
		collider->parent = parent;
	}
}


// Check if two Entities are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent Entity pointers in the correct order!)
// Params:
//	 collider = Pointer to the first Collider component.
//	 other = Pointer to the second Collider component.
void ColliderCheck(Collider* collider, Collider* other)
{
	Transform* colliderTransform = EntityGetTransform(collider->parent);
	const DGL_Vec2* colliderScale = TransformGetScale(colliderTransform);
	float radius1 = colliderScale->x / 2.0f;
	DGL_Vec2 colliderTranslation = *TransformGetTranslation(colliderTransform);

	Transform* otherTransform = EntityGetTransform(other->parent);
	const DGL_Vec2* otherScale = TransformGetScale(otherTransform);
	float radius2 = otherScale->x / 2.0f;
	DGL_Vec2 otherTranslation = *TransformGetTranslation(otherTransform);

	float lengthBetween = Vector2DDistance(&colliderTranslation, &otherTranslation);

	if (lengthBetween <= radius1 + radius2)
	{
		if (collider->handler)
		{
			collider->handler(collider->parent, other->parent);
		}
		if (other->handler)
		{
			other->handler(other->parent, collider->parent);
		}
	}
}


// Set the collision event handler for a collider.
// (Hint: This allows other components, such as Behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the Collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(Collider* collider, CollisionEventHandler handler)
{
	if (collider)
	{
		collider->handler = handler;
	}
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

