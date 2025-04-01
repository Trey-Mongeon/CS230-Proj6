//------------------------------------------------------------------------------
//
// File Name:	SandboxScene.c
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
#include "StubScene.h"
#include "Trace.h"
#include "Vector2D.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SandboxScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.

} SandboxScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SandboxSceneLoad(void);
static void SandboxSceneInit(void);
static void SandboxSceneUpdate(float dt);
static void SandboxSceneExit(void);
static void SandboxSceneUnload(void);
static void SandboxSceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static SandboxScene instance =
{
	// Initialize the base structure:
	{ "Sandbox", SandboxSceneLoad, SandboxSceneInit, SandboxSceneUpdate, SandboxSceneRender, SandboxSceneExit, SandboxSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Stub Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* SandboxSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void SandboxSceneTraceFloat(const char* text, float f)
{
	TraceMessage("Vector Test: %s = %f", text, f);
}

static void SandboxSceneTraceVector(const char* text, const Vector2D* v)
{
	TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
}

// Load any resources used by the scene.
static void SandboxSceneLoad(void)
{
}

// Initialize the entities and variables used by the scene.
static void SandboxSceneInit()
{
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void SandboxSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	Stream VecTests = StreamOpen("Data/VectorTests.txt");
	if (VecTests)
	{
		Vector2D realVector;

		Vector2DZero(&realVector);
		SandboxSceneTraceVector("Vector2DZero", &realVector);

		Vector2DSet(&realVector, 1.5f, 1.0f);
		SandboxSceneTraceVector("Vector2DSet", &realVector);

		Vector2DNeg(&realVector, &realVector);
		SandboxSceneTraceVector("Vector2DNeg", &realVector);

		Vector2DAdd(&realVector, &realVector, &realVector);
		SandboxSceneTraceVector("Vector2DAdd", &realVector);

		Vector2DSub(&realVector, &realVector, &realVector);
		SandboxSceneTraceVector("Vector2DSub", &realVector);

		StreamReadVector2D(VecTests, &realVector);
		SandboxSceneTraceVector("StreamReadVector2D", &realVector);

		Vector2DNormalize(&realVector, &realVector);
		SandboxSceneTraceVector("Vector2DNormalize", &realVector);

		float testScale = StreamReadFloat(VecTests);
		SandboxSceneTraceFloat("StreamReadFloat", testScale);

		Vector2DScale(&realVector, &realVector, testScale);
		SandboxSceneTraceVector("Vector2DScale", &realVector);

		Vector2DScaleAdd(&realVector, &realVector, testScale, &realVector);
		SandboxSceneTraceVector("Vector2DScaleAdd", &realVector);

		Vector2DScaleSub(&realVector, &realVector, testScale, &realVector);
		SandboxSceneTraceVector("Vector2DScaleSub", &realVector);

		float length = Vector2DLength(&realVector);
		SandboxSceneTraceFloat("Vector2DLength", length);

		length = Vector2DSquareLength(&realVector);
		SandboxSceneTraceFloat("Vector2DSquareLength", length);

		Vector2D tstVec0;
		Vector2D tstVec1;

		StreamReadVector2D(VecTests, &tstVec0);
		SandboxSceneTraceVector("StreamReadVector2D", &tstVec0);

		StreamReadVector2D(VecTests, &tstVec1);
		SandboxSceneTraceVector("StreamReadVector2D", &tstVec1);

		float dist = Vector2DDistance(&tstVec0, &tstVec1);
		SandboxSceneTraceFloat("Vector2DDistance", dist);

		dist = Vector2DSquareDistance(&tstVec0, &tstVec1);
		SandboxSceneTraceFloat("Vector2DSquareDistance", dist);

		float dotProd = Vector2DDotProduct(&tstVec0, &tstVec1);
		SandboxSceneTraceFloat("Vector2DDotProduct", dotProd);

		Vector2D tstVec2;
		float angle = StreamReadFloat(VecTests);
		SandboxSceneTraceFloat("StreamReadFloat", angle);

		Vector2DFromAngleDeg(&tstVec2, angle);
		SandboxSceneTraceVector("Vector2DFromAngleDeg", &tstVec2);

		angle = StreamReadFloat(VecTests);
		SandboxSceneTraceFloat("StreamReadFloat", angle);

		Vector2DFromAngleRad(&tstVec2, angle);
		SandboxSceneTraceVector("Vector2DFromAngleRad", &tstVec2);

		angle = Vector2DToAngleRad(&tstVec2);
		SandboxSceneTraceFloat("Vector2DToAngleRad", angle);

		StreamClose(&VecTests);
		
	}

	// NOTE: This call causes the engine to exit immediately.  Make sure to remove
	//   it when you are ready to test out a new scene.
	SceneSystemSetNext(NULL);
}

// Render any objects associated with the scene.
void SandboxSceneRender(void)
{
}

// Free any objects associated with the scene.
static void SandboxSceneExit()
{
}

// Unload any resources used by the scene.
static void SandboxSceneUnload(void)
{
}

