//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 7
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "DGL.h"
#include "Component.h"
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
class Physics : public Component
{
public:

	Physics();

	Physics(const Physics& other);

	~Physics();

	// Create a clone of the current component
	// (Hint: Invoke the copy constructor)
	Physics* Clone() const override
	{
		return new Physics(*this);
	}

	void Read(Stream stream);

	const Vector2D* GetAcceleration() const;

	const Vector2D* GetVelocity() const;

	float GetRotationalVelocity() const;

	const Vector2D* GetOldTranslation() const;

	void SetAcceleration(const Vector2D* acceleration);

	void SetVelocity(const Vector2D* velocity);

	void SetRotationalVelocity(float rotationalVelocity);

	void Update(float dt);

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


//------------------------------------------------------------------------------



