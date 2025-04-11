//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "DGL.h"
#include "Stream.h"
#include "Matrix2D.h"

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

Transform::Transform()
{
	scale.x = 1;
	scale.y = 1;
	isDirty = true;
	rotation = 0;
	translation.x = 0;
	translation.y = 0;
	mType = cTransform;
	mParent = NULL;
}

Transform::Transform(const Transform* other)
{
	mType = cTransform;
	mParent = NULL;

	scale.x = other->scale.x;

	scale.y = other->scale.y;

	isDirty = true;
	
	rotation = other->rotation;

	translation.x = other->translation.x;

	translation.y = other->translation.y;

	matrix = other->matrix;
}

Transform::~Transform()
{

}


// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 transform = Pointer to the Transform component.
//	 stream = The data stream used for reading.
void Transform::Read(Stream stream)
{
	StreamReadVector2D(stream, &translation);
	rotation = StreamReadFloat(stream);
	StreamReadVector2D(stream, &scale);
}

// Get the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's translation,
//		else return a NULL pointer.
const Vector2D* Transform::GetTranslation() const
{
	return &translation;
}

// Get the rotation value of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float Transform::GetRotation() const
{
	return rotation;
}

// Get the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's scale,
//		else return a NULL pointer.
const Vector2D* Transform::GetScale() const
{
	return &scale;
}


// Set the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new translation.
void Transform::SetTranslation(const Vector2D* inTranslation)
{
	if (inTranslation)
	{
		translation.x = inTranslation->x;
		translation.y = inTranslation->y;
		isDirty = TRUE;
	}
}


// Set the rotation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 rotation = The rotation value (in radians).
void Transform::SetRotation(float inRotation)
{
	rotation = inRotation;
	isDirty = TRUE;
}


// Set the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new scale.
void Transform::SetScale(const Vector2D* inScale)
{
	scale.x = inScale->x;
	scale.y = inScale->y;
	isDirty = TRUE;
}


// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
const Matrix2D* Transform::GetMatrix()
{
	if (isDirty == true)
	{
		Matrix2D matrixScale;
		Matrix2DIdentity(&matrixScale);

		Matrix2D matrixRotation;
		Matrix2DIdentity(&matrixRotation);

		Matrix2D matrixTranslation;
		Matrix2DIdentity(&matrixTranslation);

		Matrix2DScale(&matrixScale, scale.x, scale.y);
		Matrix2DRotRad(&matrixRotation, rotation);
		Matrix2DTranslate(&matrixTranslation, translation.x, translation.y);

		Matrix2D rotScale;
		Matrix2DIdentity(&rotScale);

		Matrix2DConcat(&rotScale, &matrixRotation, &matrixScale);
		Matrix2DConcat(&matrix, &matrixTranslation, &rotScale);
		isDirty = false;

	}

	return &matrix;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

