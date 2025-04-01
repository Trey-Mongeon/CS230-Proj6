//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorAsteroid.h"
#include "Behavior.h"
#include "Random.h"
#include "Entity.h"
#include "Collider.h"
#include "Teleporter.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Physics.h"
#include "ScoreSystem.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

enum AsteroidState
{
	cAsteroidInvalid = -1,
	cAsteroidIdle
};

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;


static const float asteroidSpeedMin = 50.0f;
static const float asteroidSpeedMax = 100.0f;


//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct BehaviorAsteroid
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add asteroid-specific behavior variables.
	AsteroidOrigin	origin;

} BehaviorAsteroid;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorAsteroidInit(Behavior*);
static void BehaviorAsteroidUpdate(Behavior*, float dt);
static void BehaviorAsteroidExit(Behavior* behavior);
static void BehaviorAsteroidSetPosition(BehaviorAsteroid* behavior);
static void BehaviorAsteroidSetVelocity(BehaviorAsteroid* behavior);
static void BehaviorAsteroidCollisionHandler(Entity* entity1, const Entity* entity2);


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Asteroid) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorAsteroidCreate(void)
{
	BehaviorAsteroid* asteroidBehavior = calloc(1, sizeof(BehaviorAsteroid));

	if (asteroidBehavior)
	{
		asteroidBehavior->base.memorySize = sizeof(BehaviorAsteroid);

		asteroidBehavior->base.onExit = BehaviorAsteroidExit;
		asteroidBehavior->base.onInit = BehaviorAsteroidInit;
		asteroidBehavior->base.onUpdate = BehaviorAsteroidUpdate;

		asteroidBehavior->base.stateCurr = cAsteroidInvalid;
		asteroidBehavior->base.stateNext = cAsteroidInvalid;

		return (Behavior*)asteroidBehavior;
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void BehaviorAsteroidInit(Behavior* behavior)
{
	BehaviorAsteroid* behaviorAsteroid = (BehaviorAsteroid*)behavior;

	if (behaviorAsteroid->base.stateCurr == cAsteroidIdle)
	{
		behaviorAsteroid->origin = RandomRange(0, 3);

		BehaviorAsteroidSetPosition(behaviorAsteroid);
		BehaviorAsteroidSetVelocity(behaviorAsteroid);

		Collider* asteroidCollider = EntityGetCollider(behaviorAsteroid->base.parent);

		if (asteroidCollider)
		{
			ColliderSetCollisionHandler(asteroidCollider, BehaviorAsteroidCollisionHandler);
		}
	}

}


static void BehaviorAsteroidUpdate(Behavior* behavior, float dt)
{
	TeleporterUpdateEntity(behavior->parent);
	UNREFERENCED_PARAMETER(dt);
}


static void BehaviorAsteroidExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}


static void BehaviorAsteroidSetPosition(BehaviorAsteroid* behavior)
{
	DGL_Vec2 window = DGL_Window_GetSize();

	Vector2DScale(&window, &window, 0.5f);

	Transform* entityTransform = EntityGetTransform(behavior->base.parent);

	DGL_Vec2 asteroidPosition;

	switch (behavior->origin)
	{
	case cAsteroidOriginTlc:
		asteroidPosition.x = -window.x;
		asteroidPosition.y = window.y;
		break;

	case cAsteroidOriginTrc:
		asteroidPosition.x = window.x;
		asteroidPosition.y = window.y;
		break;

	case cAsteroidOriginBlc:
		asteroidPosition.x = -window.x;
		asteroidPosition.y = -window.y;
		break;

	case cAsteroidOriginBrc:
		asteroidPosition.x = window.x;
		asteroidPosition.y = -window.y;
		break;
	}

	TransformSetTranslation(entityTransform, &asteroidPosition);
}


static void BehaviorAsteroidSetVelocity(BehaviorAsteroid* behavior)
{
	float angle = 0;
	switch (behavior->origin)
	{
	case cAsteroidOriginTlc:
		angle = RandomRangeFloat(-10, -80);
		break;

	case cAsteroidOriginTrc:
		angle = RandomRangeFloat(-100, -170);
		break;

	case cAsteroidOriginBlc:
		angle = RandomRangeFloat(10, 80);
		break;

	case cAsteroidOriginBrc:
		angle = RandomRangeFloat(100, 170);
		break;
	}

	Physics* entityPhysics = EntityGetPhysics(behavior->base.parent);

	DGL_Vec2 vecFromAngle;
	Vector2DFromAngleDeg(&vecFromAngle, angle);

	float speed = RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax);

	Vector2DScale(&vecFromAngle, &vecFromAngle, speed);

	PhysicsSetVelocity(entityPhysics, &vecFromAngle);

}


static void BehaviorAsteroidCollisionHandler(Entity* entity1, const Entity* entity2)
{
	if (entity1 && entity2)
	{
		if (EntityIsNamed(entity2, "Spaceship"))
		{
			ScoreSystemIncreaseScore(20);
			EntityDestroy(entity1);
		}
		if( EntityIsNamed(entity2, "Bullet"))
		{
			ScoreSystemIncreaseScore(20);
			EntityDestroy(entity1); 
		}
	}
}