//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Physics;
class Transform;
typedef struct DGL_Vec2 Vector2D;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class Physics
{
public:

	void Read(Stream stream);

	const Vector2D* GetAcceleration() const;

	const Vector2D* GetVelocity() const;

	float GetRotationalVelocity() const;

	const Vector2D* GetOldTranslation() const;

	void SetAcceleration(const Vector2D* acceleration);

	void SetVelocity(const Vector2D* velocity);

	void SetRotationalVelocity(float rotationalVelocity);

	void PhysicsUpdate(Transform* transform, float dt);

private:

	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Rotational velocity (in radians).
	float rotationalVelocity;
};
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


Physics* PhysicsCreate(void);

Physics* PhysicsClone(const Physics* other);

void PhysicsFree(Physics** physics);

//------------------------------------------------------------------------------



