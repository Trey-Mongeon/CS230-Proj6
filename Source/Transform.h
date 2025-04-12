//------------------------------------------------------------------------------
//
// File Name:	Transform.h
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
#include "Component.h"
#include "Vector2D.h"
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct DGL_Mat4 Matrix2D;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Transform : public Component
{
public:

	Transform();

	Transform(const Transform* other);

	~Transform();

	// Create a clone of the current component
	// (Hint: Invoke the copy constructor)
	Transform* Clone() const override
	{
		return new Transform(*this);
	}

	const Matrix2D* GetMatrix();

	const Vector2D* GetTranslation() const;

	float GetRotation() const;

	const Vector2D* GetScale() const;

	void SetTranslation(const Vector2D* translation);

	void SetRotation(float rotation);

	void SetScale(const Vector2D* scale);

	void Read(Stream stream);


private:
	// The translation (or world position) of an entity.
	Vector2D	translation;

	// The rotation (or orientation) of an entity (in radians).
	float	rotation;

	// The scale (or size) of an entity.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

	Matrix2D	matrix;

	bool	isDirty;
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------



