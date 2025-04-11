//------------------------------------------------------------------------------
//
// File Name:	Level1Scene.cpp
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
#include "Level1Scene.h"
#include "stream.h"
#include "Level2Scene.h"
#include "DemoScene.h"
#include "SandboxScene.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Entity.h"
#include "Vector2D.h"
#include "EntityFactory.h"
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "Animation.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

static const float wallDistance = 462.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);


enum MonkeyStates
{
	MonkeyInvalid = -1,
	MonkeyIdle,
	MonkeyWalk,
	MonkeyJump
};

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------


typedef struct Level1Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	int numLives;

	enum MonkeyStates monkeyState;

	Mesh* mesh;

	SpriteSource* spriteSource;

	Entity* planetEntity;



	char livesBuffer[16];


	Mesh* monkeyMesh;

	SpriteSource* monkeyIdleSource;

	SpriteSource* monkeyJumpSource;

	SpriteSource* monkeyWalkSource;

	Entity* monkeyEntity;



	Mesh* LivesTextMesh;

	SpriteSource* LivesTextSpriteSource;

	Entity* LivesText;

} Level1Scene;


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level1SceneLoad(void);
static void Level1SceneInit(void);
static void Level1SceneUpdate(float dt);
static void Level1SceneExit(void);
static void Level1SceneUnload(void);
static void Level1SceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level1Scene instance =
{

	// Initialize the base structure:
	{ "Level1", Level1SceneLoad, Level1SceneInit, Level1SceneUpdate, Level1SceneRender, Level1SceneExit, Level1SceneUnload },

	// Initialize any scene-specific variables:
	{0},
	{MonkeyInvalid}
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Stub Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* Level1SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void Level1SceneSetMonkeyState(Entity* entity, enum MonkeyStates newState)
{
	if (instance.monkeyState != newState)
	{
		instance.monkeyState = newState;

		Sprite* entitySpr = entity->GetComponent<Sprite>(Component::cSprite);
		Animation* entityAnim = entity->GetComponent<Animation>(Component::cAnimation);

		switch (newState)
		{
		case MonkeyIdle:
			entitySpr->SetMesh(instance.mesh);
			entitySpr->SetSpriteSource(instance.monkeyIdleSource);
			entityAnim->Play(1, 0.0f, false);
			break;
		case MonkeyWalk:
			entitySpr->SetMesh(instance.monkeyMesh);
			entitySpr->SetSpriteSource(instance.monkeyWalkSource);
			entityAnim->Play(8, 0.05f, true);
			break;
		case MonkeyJump:
			entitySpr->SetMesh(instance.mesh);
			entitySpr->SetSpriteSource(instance.monkeyJumpSource);
			entityAnim->Play(1, 0.0f, false);
			break;
		}
	}
}

static void Level1SceneMovementController(Entity* entity)
{
	Physics* entityPhysics = entity->GetComponent<Physics>(Component::cPhysics);
	Transform* entityTransform = entity->GetComponent<Transform>(Component::cTransform);
	
	if (entityPhysics && entityTransform)
	{
		Vector2D entityVelocity = *entityPhysics->GetVelocity();

		if (DGL_Input_KeyDown(VK_LEFT))
		{
			entityVelocity.x = -moveVelocity;
			if (instance.monkeyState != MonkeyJump)
			{
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else if (DGL_Input_KeyDown(VK_RIGHT))
		{
			entityVelocity.x = moveVelocity;
			if (instance.monkeyState != MonkeyJump)
			{
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else
		{
			entityVelocity.x = 0;
			if (instance.monkeyState != MonkeyJump)
			{
				Level1SceneSetMonkeyState(entity, MonkeyIdle);
			}
		}

		if (DGL_Input_KeyTriggered(VK_UP))
		{
			Level1SceneSetMonkeyState(entity, MonkeyJump);
			entityVelocity.y = jumpVelocity;
			entityPhysics->SetAcceleration(&gravityNormal);
		}
		Vector2D entityPos = *entityTransform->GetTranslation();

		if (entityPos.y < groundHeight)
		{
			entityPos.y = groundHeight;
			entityTransform->SetTranslation(&entityPos);
			entityVelocity.y = 0;
			entityPhysics->SetAcceleration(&gravityNone);
			Level1SceneSetMonkeyState(entity, MonkeyIdle);
		}
		entityPhysics->SetVelocity(&entityVelocity);
	}
}




static void Level1SceneBounceController(Entity* entity)
{
	Physics* ePhysics = entity->GetComponent<Physics>(Component::cPhysics);
	Transform* eTransform = entity->GetComponent<Transform>(Component::cTransform);

	if (ePhysics && eTransform)
	{
		DGL_Vec2 ePos = *eTransform->GetTranslation();
		DGL_Vec2 eVel = *ePhysics->GetVelocity();

		if (ePos.x <= -wallDistance)
		{
			ePos.x = -wallDistance;
			eVel.x = -eVel.x;
		}
		if (ePos.x >= wallDistance)
		{
			ePos.x = wallDistance;
			eVel.x = -eVel.x;
		}
		if (ePos.y <= groundHeight)
		{
			ePos.y = groundHeight + (groundHeight - ePos.y);
			eVel.y = -eVel.y;
		}
		ePhysics->SetVelocity(&eVel);
		eTransform->SetTranslation(&ePos);
	}
}


static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB)
{
	Vector2D entityAPos = *entityA->GetComponent<Transform>(Component::cTransform)->GetTranslation();
	Vector2D entityBPos = *entityB->GetComponent<Transform>(Component::cTransform)->GetTranslation();
	float dist = Vector2DSquareDistance(&entityAPos, &entityBPos);
	if (dist <= CheckSquareDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// Load any resources used by the scene.
static void Level1SceneLoad(void)
{
	Stream level1LivesFile = StreamOpen("./Data/Level1_Lives.txt");
	if (level1LivesFile)
	{
		/*sets numLives to the int read in from the file*/
		instance.numLives = StreamReadInt(level1LivesFile);
		StreamClose(&level1LivesFile);
	}

	instance.mesh = MeshCreate();
	MeshBuildQuad(instance.mesh, 0.5f, 0.5f, 1, 1, "Mesh1x1");

	instance.monkeyMesh = MeshCreate();
	MeshBuildQuad(instance.monkeyMesh, 0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");

	instance.LivesTextMesh = MeshCreate();
	MeshBuildQuad(instance.LivesTextMesh, 0.5f, 0.5f, 1.0f / 16, 1.0f / 8, "Mesh16x8");

	instance.monkeyIdleSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyIdleSource, 1, 1, "MonkeyIdle.png");

	instance.monkeyJumpSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyJumpSource, 1, 1, "MonkeyJump.png");

	instance.monkeyWalkSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyWalkSource, 3, 3, "MonkeyWalk.png");

	instance.spriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSource, 1, 1, "PlanetTexture.png");

	instance.LivesTextSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.LivesTextSpriteSource, 16, 8, "Roboto_Mono_black.png");

}

// Initialize the entities and variables used by the scene.
static void Level1SceneInit()
{
	instance.planetEntity = EntityFactoryBuild("PlanetBounce");
	if (instance.planetEntity)
	{
		instance.planetEntity->GetComponent<Sprite>(Component::cSprite)->SetMesh(instance.mesh);
		instance.planetEntity->GetComponent<Sprite>(Component::cSprite)->SetSpriteSource(instance.spriteSource);
		instance.planetEntity->GetComponent<Sprite>(Component::cSprite)->SetFrame(0);
	}
	instance.monkeyEntity = EntityFactoryBuild("Monkey");
	if (instance.monkeyEntity)
	{
		instance.monkeyState = MonkeyInvalid;
		Level1SceneSetMonkeyState(instance.monkeyEntity, MonkeyIdle);
	}
	instance.LivesText = EntityFactoryBuild("MonkeyLivesText");
	if (instance.LivesText)
	{
		Sprite* LivesTextSpr = instance.LivesText->GetComponent<Sprite>(Component::cSprite);
		LivesTextSpr->SetMesh(instance.LivesTextMesh);
		LivesTextSpr->SetSpriteSource(instance.LivesTextSpriteSource);
		sprintf_s(instance.livesBuffer, _countof(instance.livesBuffer), "Lives : %d", instance.numLives);
		LivesTextSpr->SetText(instance.livesBuffer);
	}
	const DGL_Color bg_Color = { 1.0f, 1.0f, 1.0f };
	DGL_Graphics_SetBackgroundColor(&bg_Color);

	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level1SceneUpdate(float dt)
{
	Level1SceneMovementController(instance.monkeyEntity);

	Level1SceneBounceController(instance.planetEntity);

	instance.planetEntity->Update(dt);

	instance.monkeyEntity->Update(dt);

	instance.LivesText->Update(dt);


	if (Level1SceneIsColliding(instance.planetEntity, instance.monkeyEntity))
	{
		instance.numLives -= 1;
		if (instance.numLives <= 0)
		{
			SceneSystemSetNext(Level2SceneGetInstance());
		}
		else
		{
			SceneRestart();
		}
	}
}

// Render any objects associated with the scene.
void Level1SceneRender(void)
{
	instance.planetEntity->Render();
	instance.monkeyEntity->Render();
	instance.LivesText->Render();
}

// Free any objects associated with the scene.
static void Level1SceneExit()
{
	delete instance.planetEntity;
	delete instance.monkeyEntity;
	delete instance.LivesText;
}

// Unload any resources used by the scene.
static void Level1SceneUnload(void)
{
	SpriteSourceFree(&(instance.spriteSource));
	SpriteSourceFree(&(instance.LivesTextSpriteSource));
	SpriteSourceFree(&(instance.monkeyWalkSource));
	SpriteSourceFree(&(instance.monkeyIdleSource));
	SpriteSourceFree(&(instance.monkeyJumpSource));

	MeshFree(&(instance.mesh));
	MeshFree(&(instance.monkeyMesh));
	MeshFree(&(instance.LivesTextMesh));

}


