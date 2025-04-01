//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Rotational velocity (in radians).
	float rotationalVelocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

} Physics;
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
// Dynamically allocate a new Physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Physics* PhysicsCreate(void)
{
	Physics* physicsPtr = calloc(1, sizeof(Physics));
	if (physicsPtr)
	{
		return physicsPtr;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with a Physics component.
// (NOTE: The Physics pointer must be set to NULL.)
// Params:
//	 physics = Pointer to the Physics component pointer.
void PhysicsFree(Physics** physics)
{
	if (physics && *physics)
	{
		free(*physics);
		*physics = NULL;
	}
}


// Dynamically allocate a clone of an existing Physics component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Physics* PhysicsClone(const Physics* other)
{
	if (other)
	{
		Physics* newPhysics = calloc(1, sizeof(Physics));

		if (newPhysics)
		{
			*newPhysics = *other;
			return newPhysics;
		}
	}
	return NULL;
}


// Get the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return the component's rotational velocity value,
//		else return 0.0f.
float PhysicsGetRotationalVelocity(const Physics* physics)
{
	if (physics)
	{
		return physics->rotationalVelocity;
	}
	else
	{
		return 0.0f;
	}
}


// Set the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 rotationalVelocity = The new rotational velocity.
void PhysicsSetRotationalVelocity(Physics* physics, float rotationalVelocity)
{
	if (physics)
	{
		physics->rotationalVelocity = rotationalVelocity;
	}
}


// Read the properties of a Physics component from a file.
// [NOTE: Read the acceleration and velocity values using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Physics component.
//	 stream = Pointer to the data stream used for reading.
void PhysicsRead(Physics* physics, Stream stream)
{
	if (physics && stream)
	{
		StreamReadVector2D(stream, &physics->acceleration);
		StreamReadVector2D(stream, &physics->velocity);
	}
}

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetAcceleration(const Physics* physics)
{
	if (physics)
	{
		return &physics->acceleration;
	}
	else
	{
		return NULL;
	}
}

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetVelocity(const Physics* physics)
{
	if (physics)
	{
		return &physics->velocity;
	}
	else
	{
		return NULL;
	}
}

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetOldTranslation(Physics* physics)
{
	if (physics)
	{
		return &physics->oldTranslation;
	}
	else
	{
		return NULL;
	}
}

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(Physics* physics, const Vector2D* acceleration)
{
	if (physics)
	{
		physics->acceleration.x = acceleration->x;
		physics->acceleration.y = acceleration->y;
	}
}

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(Physics* physics, const Vector2D* velocity)
{
	if (physics)
	{
		physics->velocity.x = velocity->x;
		physics->velocity.y = velocity->y;
	}
}

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides and the project instructions.
// (NOTE: This function must verify that the Physics and Transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(Physics* physics, Transform* transform, float dt)
{
	if (physics && transform)
	{
		Vector2D translation = *TransformGetTranslation(transform);
		physics->oldTranslation = translation;

		Vector2DScaleAdd(&physics->velocity, &physics->acceleration, dt, &physics->velocity);

		Vector2DScaleAdd(&translation, &physics->velocity, dt, &translation);

		TransformSetTranslation(transform, &translation);

		//rotation stuff
		float rotation = TransformGetRotation(transform);

		rotation += physics->rotationalVelocity * dt;

		TransformSetRotation(transform, rotation);
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

