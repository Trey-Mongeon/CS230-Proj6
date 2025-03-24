//------------------------------------------------------------------------------
//
// File Name:	Teleporter.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DGL.h"
#include "Teleporter.h"
#include "Vector2D.h"
#include "Entity.h"
#include "Physics.h"
#include "Transform.h"

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

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// When an Entity goes off-screen, teleport it to the other side.
// Params:
//	 entity = Pointer to the Entity to be checked.
void TeleporterUpdateEntity(Entity* entity)
{
	DGL_Vec2 window = DGL_Window_GetSize();

	Vector2DScale(&window, &window, 0.5f);

	Physics* entityPhysics = EntityGetPhysics(entity);

	const DGL_Vec2 entityVel = *PhysicsGetVelocity(entityPhysics);

	Transform* entityTransform = EntityGetTransform(entity);

	DGL_Vec2 entityTranslation = *TransformGetTranslation(entityTransform);

	if (entityVel.x > 0 && entityTranslation.x > window.x)
	{
		entityTranslation.x = -window.x;
	}
	if (entityVel.x < 0 && entityTranslation.x < -window.x)
	{
		entityTranslation.x = window.x;
	}
	if (entityVel.y > 0 && entityTranslation.y > window.y)
	{
		entityTranslation.y = -window.y;
	}
	if (entityVel.y < 0 && entityTranslation.y < -window.y)
	{
		entityTranslation.y = window.y;
	}
	TransformSetTranslation(entityTransform, &entityTranslation);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

