//------------------------------------------------------------------------------
//
// File Name:	Animation.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

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

	void Read(Animation* animation, Stream stream);

	void Play(Animation* animation, int frameCount, float frameDuration, bool isLooping);

	void Update(Animation* animation, float dt);

	bool IsDone(const Animation* animation);


private:


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



