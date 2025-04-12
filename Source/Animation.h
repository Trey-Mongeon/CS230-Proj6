//------------------------------------------------------------------------------
//
// File Name:	Animation.h
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 7
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Component.h"
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Entity Entity;
typedef class Animation Animation;
typedef class Sprite Sprite;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Animation : public Component
{

public:

	Animation();

	Animation(const Animation& other);

	~Animation();

	// Create a clone of the current component
	// (Hint: Invoke the copy constructor)
	Animation* Clone() const override
	{
		return new Animation(*this);
	}

	void Read(Stream stream);

	void Play(int frameCount, float frameDuration, bool isLooping);

	void Update(float dt);

	bool IsDone() const;



private:

	void AdvanceFrame();

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

};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------






/*----------------------------------------------------------------------------*/



