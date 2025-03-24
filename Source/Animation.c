//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Stream.h"
#include "Sprite.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Animation
{
	// Pointer to the parent Entity associated with the Animation component.
	Entity* parent;

	// The current frame being displayed.
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.
	unsigned int frameCount;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each successive frame.
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation sequence has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool isDone;

} Animation;

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

// Dynamically allocate a new Animation component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Animation* AnimationCreate(void)
{
	Animation* animationPtr = calloc(1, sizeof(Animation));
	if (animationPtr)
	{
		return animationPtr;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with an Animation component.
// (NOTE: The Animation pointer must be set to NULL.)
// Params:
//	 animation = Pointer to the Animation pointer.
void AnimationFree(Animation** animation)
{
	if (animation && *animation)
	{
		free(*animation);
		*animation = NULL;
	}
}


// Dynamically allocate a clone of an existing Animation component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Animation* AnimationClone(const Animation* other)
{
	if (other)
	{
		Animation* newAnim = calloc(1, sizeof(Animation));

		if (newAnim)
		{
			*newAnim = *other;
			return newAnim;
		}
	}
	return NULL;
}


// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = The data stream used for reading.
void AnimationRead(Animation* animation, Stream stream)
{
	animation->frameIndex = StreamReadInt(stream);
	animation->frameCount = StreamReadInt(stream);

	animation->frameDelay = StreamReadFloat(stream);
	animation->frameDuration = StreamReadFloat(stream);

	animation->isRunning = StreamReadBoolean(stream);
	animation->isLooping = StreamReadBoolean(stream);
}

// Set the parent Entity for an Animation component.
// Params:
//	 animation = Pointer to the Animation component.
//	 parent = Pointer to the parent Entity.
void AnimationSetParent(Animation* animation, Entity* parent)
{
	animation->parent = parent;
}

// Play a simple animation sequence [0 .. frameCount - 1].
// (Hint: This function must initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the Animation component.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to display each frame (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(Animation* animation, int frameCount, float frameDuration, bool isLooping)
{
	if (animation)
	{
		animation->frameCount = frameCount;                      ///// TEST THIS FUNCTION THOROUGHLY
		animation->frameDuration = frameDuration;
		animation->isLooping = isLooping;
		animation->frameDelay = frameDuration;
		animation->frameIndex = 0;
		animation->isDone = false;
		animation->isRunning = true;

		SpriteSetFrame(EntityGetSprite(animation->parent), animation->frameIndex); /// ********************************************* CHECK THIS TOO
	}
	else
	{
		return;
	}
}


static void AnimationAdvanceFrame(Animation* animation);


// Update the animation.
// Params:
//	 animation = Pointer to the Animation component.
//	 dt = Change in time (in seconds) since the last game loop.
void AnimationUpdate(Animation* animation, float dt)
{
	if (animation)
	{
		animation->isDone = false;
		if(animation->isRunning)
		{
			animation->frameDelay -= dt;

		if (animation->frameDelay <= 0)
		{
			AnimationAdvanceFrame(animation);
		}
		}
	}
	else
	{
		return;
	}
}

static void AnimationAdvanceFrame(Animation* animation)
{
	if (animation && animation->parent) // SAYS VALID POINTERS ON THE POWERPOINT
	{
		animation->frameIndex += 1;
		if (animation->frameIndex >= animation->frameCount)
		{
			if (animation->isLooping)
			{
				animation->frameIndex = 0;
			}

			else
			{
				animation->frameIndex = animation->frameCount - 1;
				animation->isRunning = false;
			}
			animation->isDone = true;
		}

		if (animation->isRunning)
		{
			SpriteSetFrame(EntityGetSprite(animation->parent), animation->frameIndex);
			animation->frameDelay += animation->frameDuration;
		}
		else
		{
			animation->frameDelay = 0;
		}
	}
		return;
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the Animation component.
// Returns:
//	 If the Animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool AnimationIsDone(const Animation* animation)
{
	if (animation)
	{
		return animation->isDone;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

