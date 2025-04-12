//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------------

class Entity;


class Component
{

	// Public Functions:
public:

enum TypeEnum
{
	cInvalid = -1,
	cSprite,
	cTransform,
	cAnimation,
	cPhysics
};

	Component()
	{
		mType = cInvalid;
		mParent = NULL;
	}


	virtual ~Component()
	{

	}


	Component(const Component& other)
	{
		mType = other.mType;
		mParent = other.mParent;
	}


	__inline Component::TypeEnum Type() const 
	{
		return mType; 
	}

	__inline void SetParent(Entity* parent) 
	{ 
		mParent = parent; 
	}

	__inline Entity* GetParent() const 
	{
		return mParent; 
	}

	// Clone is used to invoke the C++ copy constructor.
	virtual Component* Clone() const = 0;

	// Component-specific render code.
	virtual void Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Component-specific render code.
	virtual void Render() const 
	{

	};

	// NOTE: This function must be declared as static.
	static bool Component::Comparator(const Component* left, const Component* right)
	{
		return (left->Type() < right->Type());
	}



	// Private Variables:
protected:
	// The component type. 
	TypeEnum mType;

	// The component's parent Entity.
	Entity* mParent;

};

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------



