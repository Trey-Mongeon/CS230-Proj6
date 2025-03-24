//------------------------------------------------------------------------------
//
// File Name:	BehaviorHudText.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorHudText.h"
#include "ScoreSystem.h"
#include "Behavior.h"
#include "Stream.h"
#include "Entity.h"
#include "Sprite.h"
#include "ScoreSystem.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

typedef enum HudTextStates
{
	cHudTextInvalid = -1,	// HUD Text has not yet been initialized.
	cHudTextIdle,			// HUD Text will normally remain static.

} HudTextStates;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct BehaviorHudText
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add HUD Text-specific behavior variables.

	// The index used to access values from the Score System.
	ScoreSystemId scoreSystemId;

	// The format string to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: %d")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char formatString[32];

	// The buffer to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: 9001")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char displayString[32];

	// The value currently being displayed by the HUD Text object.
	// (NOTE: This value can be compared with *watchValue to determine when the text must be updated.)
	// (NOTE: Make sure to update this value each time the text is updated.)
	unsigned displayValue;

} BehaviorHudText;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorHudTextInit(Behavior* behavior);
static void BehaviorHudTextUpdate(Behavior* behavior, float dt);
static void BehaviorHudTextExit(Behavior* behavior);
static void BehaviorHudTextUpdateText(BehaviorHudText* behavior);


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (HUD Text) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorHudTextCreate(void)
{
	BehaviorHudText* hudBehavior = calloc(1, sizeof(BehaviorHudText));

	if (hudBehavior)
	{
		hudBehavior->base.memorySize = sizeof(BehaviorHudText);

		hudBehavior->base.onExit = BehaviorHudTextExit;
		hudBehavior->base.onUpdate = BehaviorHudTextUpdate;
		hudBehavior->base.onInit = BehaviorHudTextInit;

		hudBehavior->base.stateCurr = cHudTextInvalid;
		hudBehavior->base.stateNext = cHudTextInvalid;

		hudBehavior->scoreSystemId = SsiInvalid;

		return (Behavior*)hudBehavior;
	}
	else
	{
		return NULL;
	}
}


// Read the properties of a Behavior component from a file.
// (NOTE: Read the base Behavior values using BehaviorRead.)
// (NOTE: Read the formatString using StreamReadToken.)
// (NOTE: Read the scoreSystemId using StreamReadInt.)
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
void BehaviorHudTextRead(Behavior* behavior, Stream stream)
{
	if (behavior)
	{
		BehaviorRead(behavior, stream);

		BehaviorHudText* hudBehavior = (BehaviorHudText*)behavior;

		strcpy_s(hudBehavior->formatString, sizeof(hudBehavior->formatString), StreamReadToken(stream));
		hudBehavior->scoreSystemId = StreamReadInt(stream);
	}
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void BehaviorHudTextInit(Behavior* behavior)
{
	BehaviorHudText* hudBehavior = (BehaviorHudText*)behavior;
	BehaviorHudTextUpdateText(hudBehavior);

	SpriteSetText(EntityGetSprite(behavior->parent), hudBehavior->displayString);
}


static void BehaviorHudTextUpdate(Behavior* behavior, float dt)
{
	BehaviorHudText* hudBehavior = (BehaviorHudText*)behavior;
	unsigned int value = ScoreSystemGetValue(hudBehavior->scoreSystemId);
	if (value != hudBehavior->displayValue)
	{
		BehaviorHudTextUpdateText(hudBehavior);
	}
	UNREFERENCED_PARAMETER(dt);
}


static void BehaviorHudTextExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}


static void BehaviorHudTextUpdateText(BehaviorHudText* behavior)
{
	BehaviorHudText* hudBehavior = (BehaviorHudText*)behavior;

	if (hudBehavior->scoreSystemId != SsiInvalid)
	{
		hudBehavior->displayValue = ScoreSystemGetValue(hudBehavior->scoreSystemId);

		sprintf_s(hudBehavior->displayString, _countof(hudBehavior->displayString), hudBehavior->formatString, hudBehavior->displayValue);
	}
}