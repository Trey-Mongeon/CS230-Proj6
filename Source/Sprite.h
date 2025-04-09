//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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
#include "Component.h"
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Mesh Mesh;
typedef class SpriteSource SpriteSource;
typedef class Transform Transform;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


class Sprite : public Component
{

	// Constructors/Destructors:
public:

	Sprite();


	Sprite(const Sprite& other);


	~Sprite(void);


	// Public Functions:
	
	// Create a clone of the current component
	// (Hint: Invoke the copy constructor)
	Sprite* Clone() const override
	{
		return new Sprite(*this);
	}
	//...


	void Read(Stream stream);


	void Render(Transform* transform) const;


	float GetAlpha();


	void SetAlpha(float alpha);


	void SetFrame(unsigned int frameIndex);


	void SetMesh(const Mesh* mesh);


	void SetSpriteSource(const SpriteSource* spriteSource);


	void SetText(const char* text);

private:


	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	const SpriteSource* spriteSource;

	// The mesh used to draw the sprite.
	const Mesh* mesh;

	// Zero-terminated string used to display sprite text.
	const char* text;


};



/*----------------------------------------------------------------------------*/


