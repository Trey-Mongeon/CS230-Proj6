//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "Entity.h"
#include "Teleporter.h"
#include "Collider.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

enum bulletState
{
	cBulletInvalid = -1,
	cBulletIdle
};

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

static void BehaviorBulletOnInit(Behavior* behavior);
static void BehaviorBulletOnUpdate(Behavior* behavior, float dt);
static void BehaviorBulletOnExit(Behavior* behavior);
static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt);

static void BehaviorBulletCollisionHandler(Entity* entity1, Entity* entity2);


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorBulletCreate(void)
{
	Behavior* bulletBehavior = calloc(1, sizeof(Behavior));

	if (bulletBehavior)
	{
		bulletBehavior->stateCurr = cBulletInvalid;
		bulletBehavior->stateNext = cBulletInvalid;
		bulletBehavior->onInit = BehaviorBulletOnInit;
		bulletBehavior->onUpdate = BehaviorBulletOnUpdate;
		bulletBehavior->onExit = BehaviorBulletOnExit;
		bulletBehavior->memorySize = sizeof(Behavior);

		return bulletBehavior;
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorBulletOnInit(Behavior* behavior)
{
	if (behavior->stateCurr == cBulletIdle)
	{
		Collider* entityCollider = EntityGetCollider(behavior->parent);
		if (entityCollider)
		{
			ColliderSetCollisionHandler(entityCollider, BehaviorBulletCollisionHandler);
		}
	}
}


static void BehaviorBulletOnUpdate(Behavior* behavior, float dt)
{
	switch (behavior->stateCurr)
	{
	case cBulletIdle:
		BehaviorBulletUpdateLifeTimer(behavior, dt);
	}
	TeleporterUpdateEntity(behavior->parent);
}


static void BehaviorBulletOnExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}


static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt)
{
	if (behavior->timer > 0)
	{
		behavior->timer -= dt;
		
		if (behavior->timer <= 0)
		{
			EntityDestroy(behavior->parent);
		}
	}
}


static void BehaviorBulletCollisionHandler(Entity* entity1, Entity* entity2)
{
	if (entity1 && entity2)
	{
		if (EntityIsNamed(entity2, "Asteroid"))
		{
			EntityDestroy(entity1);
		}
	}
}