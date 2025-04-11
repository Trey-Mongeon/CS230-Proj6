//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
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
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
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

Physics::Physics()
{
	oldTranslation = { 0, 0 };

	acceleration = { 0, 0 };

	velocity = { 0, 0 };

	rotationalVelocity = 0;

	mParent = NULL;

	mType = cPhysics;
}


Physics::Physics(const Physics& other)
{
	oldTranslation = other.oldTranslation;

	acceleration = other.acceleration;

	velocity = other.velocity;

	rotationalVelocity = other.rotationalVelocity;
}


Physics::~Physics()
{

}


// Get the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return the component's rotational velocity value,
//		else return 0.0f.
float Physics::GetRotationalVelocity() const
{
	return rotationalVelocity;
}


// Set the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 rotationalVelocity = The new rotational velocity.
void Physics::SetRotationalVelocity(float inRotationalVelocity)
{
	rotationalVelocity = inRotationalVelocity;
}


// Read the properties of a Physics component from a file.
// [NOTE: Read the acceleration and velocity values using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Physics component.
//	 stream = Pointer to the data stream used for reading.
void Physics::Read(Stream stream)
{
	if (stream)
	{
		StreamReadVector2D(stream, &acceleration);
		StreamReadVector2D(stream, &velocity);
	}
}

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D* Physics::GetAcceleration() const
{
	return &acceleration;
}

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D* Physics::GetVelocity() const
{
	return &velocity;
}

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D* Physics::GetOldTranslation() const
{
	return &oldTranslation;
}

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void Physics::SetAcceleration(const Vector2D* inAcceleration)
{
	acceleration.x = inAcceleration->x;
	acceleration.y = inAcceleration->y;
}

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void Physics::SetVelocity(const Vector2D* inVelocity)
{
		velocity.x = inVelocity->x;
		velocity.y = inVelocity->y;
}

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides and the project instructions.
// (NOTE: This function must verify that the Physics and Transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void Physics::Update(float dt)
{
	Transform* transform = GetParent()->GetComponent<Transform>(cTransform);

	Vector2D translation = *transform->GetTranslation();
	oldTranslation = translation;

	Vector2DScaleAdd(&velocity, &acceleration, dt, &velocity);

	Vector2DScaleAdd(&translation, &velocity, dt, &translation);

	transform->SetTranslation(&translation);

	//rotation stuff
	float rotation = transform->GetRotation();

	rotation += rotationalVelocity * dt;

	transform->SetRotation(rotation);
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

