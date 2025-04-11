//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
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


Animation::Animation()
{
	mParent = NULL;

	mType = cAnimation;

	frameIndex = 0;

	frameCount = 0;

	frameDelay = 0;

	frameDuration = 0;

	isRunning = false;

	isLooping = false;

	isDone = false;
}

Animation::~Animation()
{

}

Animation::Animation(const Animation& other)
{
	mParent = other.mParent;

	mType = other.mType;

	frameIndex = other.frameIndex;

	frameCount = other.frameCount;

	frameDelay = other.frameDelay;

	frameDuration = other.frameDuration;

	isRunning = other.isRunning;

	isLooping = other.isLooping;

	isDone = other.isDone;
}


// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = The data stream used for reading.
void Animation::Read(Stream stream)
{
	frameIndex = StreamReadInt(stream);
	frameCount = StreamReadInt(stream);

	frameDelay = StreamReadFloat(stream);
	frameDuration = StreamReadFloat(stream);

	isRunning = StreamReadBoolean(stream);
	isLooping = StreamReadBoolean(stream);
}


// Play a simple animation sequence [0 .. frameCount - 1].
// (Hint: This function must initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the Animation component.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to display each frame (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(int inFrameCount, float inFrameDuration, bool inIsLooping)
{
		frameCount = inFrameCount;
		frameDuration = inFrameDuration;
		isLooping = inIsLooping;
		frameDelay = inFrameDuration;
		frameIndex = 0;
		isDone = false;
		isRunning = true;

		Sprite* sprite = GetParent()->GetComponent<Sprite>(cSprite);
		sprite->SetFrame(frameIndex);
}


// Update the animation.
// Params:
//	 animation = Pointer to the Animation component.
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update(float dt)
{
	isDone = false;
	if(isRunning)
	{
		frameDelay -= dt;

		if (frameDelay <= 0)
		{
			AdvanceFrame();
		}
	}
}

void Animation::AdvanceFrame()
{
	if (GetParent())
	{
		frameIndex += 1;
		if (frameIndex >= frameCount)
		{
			if (isLooping)
			{
				frameIndex = 0;
			}

			else
			{
				frameIndex = frameCount - 1;
				isRunning = false;
			}
			isDone = true;
		}

		if (isRunning)
		{
			Sprite* sprite = GetParent()->GetComponent<Sprite>(cSprite);
			sprite->SetFrame(frameIndex);

			frameDelay += frameDuration;
		}
		else
		{
			frameDelay = 0;
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
bool Animation::IsDone() const
{
	return isDone;
}
