//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"
#include "Transform.h"
#include "Entity.h"
#include "DGL.h"
#include "Vector2D.h"
#include "ColliderCircle.h"
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

bool ColliderIsColliding(const Collider* collider, const Collider* other);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


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
		Collider* newCollider = calloc(1, other->memorySize);

		if (newCollider)
		{
			memcpy(newCollider, other, other->memorySize);
			newCollider->parent = NULL;
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
void ColliderCheck(const Collider* collider, const Collider* other)
{
	if (ColliderIsColliding(collider, other))
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


bool ColliderIsColliding(const Collider* collider, const Collider* other)
{
	switch (collider->type)
	{
	case ColliderTypeCircle:
		if (other->type == ColliderTypeCircle)
		{
			return ColliderCircleIsCollidingWithCircle(collider, other);
		}
		else if(other->type == ColliderTypeLine)
		{
			//call ColliderCircleCollidingWithLine
		}
		break;

	case ColliderTypeLine:
		if (other->type == ColliderTypeCircle)
		{
			return ColliderLineIsCollidingWithCircle(collider, other);
		}
		else if (other->type == ColliderTypeLine)
		{
			return false;
		}
		break;

	default:
		return false;
		break;
	}
	return false;
}
