//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include "Behavior.h"
#include "DGL.h"
#include "Entity.h"
#include "Transform.h"
#include "Physics.h"
#include "Vector2D.h"
#include "EntityFactory.h"
#include "Scene.h"
#include "Teleporter.h"
#include "Collider.h"
#include "Sprite.h"

#define PI 3.14159

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

enum spaceshipState
{
	cSpaceshipInvalid = -1,
	cSpaceshipIdle,
	cSpaceshipThrust,
	cSpaceshipDead
};

static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = (float)PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.25f;
static const float spaceshipWeaponBulletSpeed = 750.0f;
static const float spaceshipDeathDuration = 3.0f;


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

static void BehaviorSpaceshipOnInit(Behavior* behavior);
static void BehaviorSpaceshipOnUpdate(Behavior* behavior, float dt);
static void BehaviorSpaceshipOnExit(Behavior* behavior);
static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior);
static void BehaviorSpaceshipCollisionHandler(Entity* entity1, Entity* entity2);


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorSpaceshipCreate(void)
{
	Behavior* spaceshipBehavior = calloc(1, sizeof(Behavior));
	if (spaceshipBehavior)
	{
		spaceshipBehavior->stateCurr = cSpaceshipInvalid;
		spaceshipBehavior->stateNext = cSpaceshipInvalid;

		spaceshipBehavior->onInit = BehaviorSpaceshipOnInit;
		spaceshipBehavior->onUpdate = BehaviorSpaceshipOnUpdate;
		spaceshipBehavior->onExit = BehaviorSpaceshipOnExit;
		spaceshipBehavior->memorySize = sizeof(Behavior);

		return spaceshipBehavior;
	}
	return NULL;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipOnInit(Behavior* behavior)
{
	if (behavior->stateCurr == cSpaceshipIdle)
	{
		Collider* entityCollider = EntityGetCollider(behavior->parent);
		if (entityCollider)
		{
			ColliderSetCollisionHandler(entityCollider, BehaviorSpaceshipCollisionHandler);
		}
	}
	if (behavior->stateCurr == cSpaceshipDead)
	{
		behavior->timer = spaceshipDeathDuration;
	}
}


static void BehaviorSpaceshipOnUpdate(Behavior* behavior, float dt)
{

	switch (behavior->stateCurr)
	{
		case cSpaceshipIdle:

			BehaviorSpaceshipUpdateRotation(behavior, dt);
			BehaviorSpaceshipUpdateWeapon(behavior, dt);

			if (DGL_Input_KeyDown(VK_UP))
			{
				behavior->stateNext = cSpaceshipThrust;

			}
			break;

		case cSpaceshipThrust:

			BehaviorSpaceshipUpdateRotation(behavior, dt);
			BehaviorSpaceshipUpdateVelocity(behavior, dt);
			BehaviorSpaceshipUpdateWeapon(behavior, dt);

			if (!DGL_Input_KeyDown(VK_UP))
			{
				behavior->stateNext = cSpaceshipIdle;
			}
			break;

		case cSpaceshipDead:

			behavior->timer -= dt;
			if (behavior->timer <= 0)
			{
				SceneRestart();
			}
			//************** DEATH IMPLEMENTATION

			Physics* parentPhysics = EntityGetPhysics(behavior->parent);

			float rotVel = PhysicsGetRotationalVelocity(parentPhysics);

			rotVel = spaceshipTurnRateMax * 5;

			Transform* parentTransform = EntityGetTransform(behavior->parent);

			float shipRot = TransformGetRotation(parentTransform);

			Vector2D vecFromShipRot;
			Vector2DFromAngleRad(&vecFromShipRot, shipRot);

			const Vector2D* velocity = PhysicsGetVelocity(parentPhysics);

			Vector2D newVelocity;

			Vector2DScaleAdd(&newVelocity, &vecFromShipRot, spaceshipAcceleration * dt * 5, velocity);

			float speed = Vector2DLength(&newVelocity);

			Vector2DScale(&newVelocity, &newVelocity, speed / speed - 2.4f);  // Ship splits in 2 on purpose

			PhysicsSetVelocity(parentPhysics, &newVelocity);

			float alpha = SpriteGetAlpha(EntityGetSprite(behavior->parent));

			alpha -= dt * 2;

			SpriteSetAlpha(EntityGetSprite(behavior->parent), alpha);

			PhysicsSetRotationalVelocity(parentPhysics, rotVel);

	}
	TeleporterUpdateEntity(behavior->parent);
}


static void BehaviorSpaceshipOnExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}


static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (DGL_Input_KeyDown(VK_LEFT))
	{
		Physics* parentPhysics = EntityGetPhysics(behavior->parent);

		float rotVel = PhysicsGetRotationalVelocity(parentPhysics);

		rotVel = spaceshipTurnRateMax;

		PhysicsSetRotationalVelocity(parentPhysics, rotVel);
	}
	else if (DGL_Input_KeyDown(VK_RIGHT))
	{
		Physics* parentPhysics = EntityGetPhysics(behavior->parent);

		float rotVel = PhysicsGetRotationalVelocity(parentPhysics);

		rotVel = -spaceshipTurnRateMax;

		PhysicsSetRotationalVelocity(parentPhysics, rotVel);
	}
	else
	{
		Physics* parentPhysics = EntityGetPhysics(behavior->parent);

		float rotVel = 0.0f;
		PhysicsSetRotationalVelocity(parentPhysics, rotVel);
	}
}


static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt)
{
	Transform* parentTransform = EntityGetTransform(behavior->parent);

	Physics* parentPhysics = EntityGetPhysics(behavior->parent);

	if (parentTransform && parentPhysics)
	{
		float shipRot = TransformGetRotation(parentTransform);

		Vector2D vecFromShipRot;
		Vector2DFromAngleRad(&vecFromShipRot, shipRot);

		const Vector2D *velocity = PhysicsGetVelocity(parentPhysics);

		Vector2D newVelocity;

		Vector2DScaleAdd(&newVelocity, &vecFromShipRot, spaceshipAcceleration * dt, velocity);

		float speed = Vector2DLength(&newVelocity);

		if (speed > spaceshipSpeedMax)
		{
			Vector2DScale(&newVelocity, &newVelocity, spaceshipSpeedMax / speed);
		}
		PhysicsSetVelocity(parentPhysics, &newVelocity);
	}
	
}


static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt)
{
	if (behavior->timer > 0)
	{
		behavior->timer -= dt;

		if (behavior->timer < 0)
		{
			behavior->timer = 0;
		}
	}

	if (DGL_Input_KeyDown(VK_SPACE))
	{
		if (behavior->timer <= 0)
		{
			BehaviorSpaceshipSpawnBullet(behavior);
			behavior->timer = spaceshipWeaponCooldownTime;
		}
	}
}


static void BehaviorSpaceshipSpawnBullet(Behavior* behavior)
{
	Entity* bullet = EntityFactoryBuild("Bullet");

	if (bullet)
	{
		Transform* parentTransform = EntityGetTransform(behavior->parent);
		const DGL_Vec2 spaceshipPosition = *TransformGetTranslation(parentTransform);
		float shipRot = TransformGetRotation(parentTransform);

		Transform* bulletTransform = EntityGetTransform(bullet);
		TransformSetRotation(bulletTransform, shipRot);
		TransformSetTranslation(bulletTransform, &spaceshipPosition);

		DGL_Vec2 vecFromShipRot;
		Vector2DFromAngleRad(&vecFromShipRot, shipRot);

		Physics* bulletPhysics = EntityGetPhysics(bullet);

		Vector2DScale(&vecFromShipRot, &vecFromShipRot, spaceshipWeaponBulletSpeed);

		PhysicsSetVelocity(bulletPhysics, &vecFromShipRot);

		SceneAddEntity(bullet);
	}
}


static void BehaviorSpaceshipCollisionHandler(Entity* entity1, Entity* entity2)
{
	if (entity1 && entity2)
	{
		if (EntityIsNamed(entity2, "Asteroid"))
		{ 
			Behavior* entity1Behavior = EntityGetBehavior(entity1);
			entity1Behavior->stateNext = cSpaceshipDead;
		}
	}
}