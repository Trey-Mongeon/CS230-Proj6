//------------------------------------------------------------------------------
//
// File Name:	OmegaScene.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "OmegaScene.h"
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

static void OmegaSceneLoad(void);
static void OmegaSceneInit(void);
static void OmegaSceneUpdate(float dt);
static void OmegaSceneExit(void);
static void OmegaSceneUnload(void);
static void OmegaSceneRender(void);

static void OmegaSceneSpawnAsteroidWave(void);
static void OmegaSceneSpawnAsteroid(void);


//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static AsteroidsScene instance =
{
	// Initialize the base structure:
	{ "Asteroids", OmegaSceneLoad, OmegaSceneInit, OmegaSceneUpdate, OmegaSceneRender, OmegaSceneExit, OmegaSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Omega Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* OmegaSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void OmegaSceneLoad(void)
{
	ScoreSystemClear();
}

// Initialize the entities and variables used by the scene.
static void OmegaSceneInit()
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
	
	OmegaSceneSpawnAsteroidWave();

	DGL_Color bgColor = { 0,0,0,0 };
	DGL_Graphics_SetBackgroundColor(&bgColor);

	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void OmegaSceneUpdate(float dt)
{
	Entity* foundEntity = SceneFindEntityByName("Asteroid");

	if (!foundEntity)
	{
		OmegaSceneSpawnAsteroidWave();
	}
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);
}

// Render any objects associated with the scene.
void OmegaSceneRender(void)
{
}

// Free any objects associated with the scene.
static void OmegaSceneExit()
{
}

// Unload any resources used by the scene.
static void OmegaSceneUnload(void)
{
}


static void OmegaSceneSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWave();
	for (unsigned int i = 0; i < instance.asteroidSpawnCount; ++i)
	{
		OmegaSceneSpawnAsteroid();
	}
	if (instance.asteroidSpawnCount < cAsteroidSpawnMaximum)
	{
		++instance.asteroidSpawnCount;
	}
}


static void OmegaSceneSpawnAsteroid(void)
{
	Entity* Asteroid = EntityFactoryBuild("Asteroid");
	if (Asteroid)
	{
		SceneAddEntity(Asteroid);
	}
}
