//------------------------------------------------------------------------------
//
// File Name:	AsteroidsScene.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "AsteroidsScene.h"
#include "DGL.h"
#include "EntityFactory.h"
#include "ScoreSystem.h"
#include "Scene.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const unsigned cAsteroidSpawnInitial = 8;
static const unsigned cAsteroidSpawnMaximum = 20;


//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct AsteroidsScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	unsigned asteroidSpawnCount;

} AsteroidsScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void AsteroidsSceneLoad(void);
static void AsteroidsSceneInit(void);
static void AsteroidsSceneUpdate(float dt);
static void AsteroidsSceneExit(void);
static void AsteroidsSceneUnload(void);
static void AsteroidsSceneRender(void);

static void AsteroidsSceneSpawnAsteroidWave(void);
static void AsteroidsSceneSpawnAsteroid(void);


//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static AsteroidsScene instance =
{
	// Initialize the base structure:
	{ "Asteroids", AsteroidsSceneLoad, AsteroidsSceneInit, AsteroidsSceneUpdate, AsteroidsSceneRender, AsteroidsSceneExit, AsteroidsSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Asteroids Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* AsteroidsSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void AsteroidsSceneLoad(void)
{
	ScoreSystemClear();
}

// Initialize the entities and variables used by the scene.
static void AsteroidsSceneInit()
{
	Entity* spaceshipEntity = EntityFactoryBuild("Spaceship");
	if (spaceshipEntity)
	{
		SceneAddEntity(spaceshipEntity);
	}

	Entity* asteroidsScoreEntity = EntityFactoryBuild("AsteroidsScore");
	if (asteroidsScoreEntity)
	{
		SceneAddEntity(asteroidsScoreEntity);
	}

	Entity* asteroidsHighScoreEntity = EntityFactoryBuild("AsteroidsHighScore");
	if (asteroidsHighScoreEntity)
	{
		SceneAddEntity(asteroidsHighScoreEntity);
	}

	Entity* asteroidsWaveEntity = EntityFactoryBuild("AsteroidsWave");
	if (asteroidsWaveEntity)
	{
		SceneAddEntity(asteroidsWaveEntity);
	}

	ScoreSystemReset();

	instance.asteroidSpawnCount = cAsteroidSpawnInitial;
	
	AsteroidsSceneSpawnAsteroidWave();

	DGL_Color bgColor = { 0,0,0,0 };
	DGL_Graphics_SetBackgroundColor(&bgColor);

	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void AsteroidsSceneUpdate(float dt)
{
	Entity* foundEntity = SceneFindEntityByName("Asteroid");

	if (!foundEntity)
	{
		AsteroidsSceneSpawnAsteroidWave();
	}
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);
}

// Render any objects associated with the scene.
void AsteroidsSceneRender(void)
{
}

// Free any objects associated with the scene.
static void AsteroidsSceneExit()
{
}

// Unload any resources used by the scene.
static void AsteroidsSceneUnload(void)
{
}


static void AsteroidsSceneSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWave();
	for (unsigned int i = 0; i < instance.asteroidSpawnCount; ++i)
	{
		AsteroidsSceneSpawnAsteroid();
	}
	if (instance.asteroidSpawnCount < cAsteroidSpawnMaximum)
	{
		++instance.asteroidSpawnCount;
	}
}


static void AsteroidsSceneSpawnAsteroid(void)
{
	Entity* Asteroid = EntityFactoryBuild("Asteroid");
	if (Asteroid)
	{
		SceneAddEntity(Asteroid);
	}
}
