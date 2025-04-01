//------------------------------------------------------------------------------
//
// File Name:	Level2Scene.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "Level2Scene.h"
#include "Level1Scene.h"
#include "Stream.h"
#include "SandboxScene.h"
#include "DemoScene.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "DGL.h"
#include "Sprite.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Physics.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const float spaceshipSpeed = 500.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level2Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	Mesh* lvl2Mesh;
	Entity* lvl2Entity;

} Level2Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level2SceneLoad(void);
static void Level2SceneInit(void);
static void Level2SceneUpdate(float dt);
static void Level2SceneExit(void);
static void Level2SceneUnload(void);
static void Level2SceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level2Scene instance =
{
	// Initialize the base structure:
	{ "Level2", Level2SceneLoad, Level2SceneInit, Level2SceneUpdate, Level2SceneRender, Level2SceneExit, Level2SceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Stub Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* Level2SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void Level2SceneMovementController(Entity* entity)
{
	Physics* entityPhysics = EntityGetPhysics(entity);
	Sprite* entitySprite = EntityGetSprite(entity);

	if (!(entityPhysics && entitySprite))
	{
		return;
	}

	Vector2D screenMousePos = DGL_Input_GetMousePosition();
	Vector2D mousePos = DGL_Camera_ScreenCoordToWorld(&screenMousePos);

	Transform* entityTransform = EntityGetTransform(entity);
	Vector2D shipPos = *TransformGetTranslation(entityTransform);
	Vector2D vecToMouse;
	Vector2DSub(&vecToMouse, &mousePos, &shipPos);

	Vector2D normalVecToMouse;
	Vector2DNormalize(&normalVecToMouse, &vecToMouse);

	TransformSetRotation(entityTransform, Vector2DToAngleRad(&normalVecToMouse));

	Vector2D scaledNormalVec;
	Vector2DScale(&scaledNormalVec, &normalVecToMouse, spaceshipSpeed);
	PhysicsSetVelocity(entityPhysics, &scaledNormalVec);

}

// Load any resources used by the scene.
	
static void Level2SceneLoad(void)
{
	/*
	Stream level2LivesFile = StreamOpen("./Data/Level2_Lives.txt");
	if (level2LivesFile)
	{
		sets numLives to the int read in from the file
		instance.numLives = StreamReadInt(level2LivesFile);
		StreamClose(&level2LivesFile);
	}
	*/

	instance.lvl2Mesh = MeshCreate();
	MeshBuildSpaceship(instance.lvl2Mesh);
}

// Initialize the entities and variables used by the scene.
static void Level2SceneInit()
{
	instance.lvl2Entity = EntityFactoryBuild("SpaceshipHoming");

	if (instance.lvl2Entity)
	{
		SpriteSetMesh(EntityGetSprite(instance.lvl2Entity), instance.lvl2Mesh);
	}
	DGL_Color black = { 0.0f, 0.0f, 0.0f, 1.0f };
	DGL_Graphics_SetBackgroundColor(&black);

	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level2SceneUpdate(float dt)
{
	Level2SceneMovementController(instance.lvl2Entity);
	EntityUpdate(instance.lvl2Entity, dt);

	if (DGL_Input_KeyTriggered('Z'))
	{
		SpriteSetAlpha(EntityGetSprite(instance.lvl2Entity), 0.5f);
	}
	if (DGL_Input_KeyTriggered('X'))
	{
		SpriteSetAlpha(EntityGetSprite(instance.lvl2Entity), 1.0f);
	}
}

// Render any objects associated with the scene.
void Level2SceneRender(void)
{
	EntityRender(instance.lvl2Entity);
}

// Free any objects associated with the scene.
static void Level2SceneExit()
{
	EntityFree(&instance.lvl2Entity);
}

// Unload any resources used by the scene.
static void Level2SceneUnload(void)
{
	MeshFree(&instance.lvl2Mesh);
}

