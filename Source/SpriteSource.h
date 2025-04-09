//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
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

typedef struct DGL_Texture DGL_Texture;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class SpriteSource
{

	// Constructors/Destructors:
public:
	SpriteSource();


	~SpriteSource();


	// Public Functions:


	void LoadTexture(int inNumCols, int inNumRows, const char* textureName);


	void Read(Stream stream);


	bool IsNamed(const char* name) const;


	unsigned GetFrameCount() const;


	void GetUV(unsigned int frameIndex, float* u, float* v) const;


	void SetTexture() const;


	void SetTextureOffset(unsigned frameIndex) const;


private:


	char name[32];

	// The dimensions of the sprite sheet.
	// (Hint: These should be initialized to (1, 1).)
	int	numRows;
	int	numCols;

	// Pointer to a texture resource created using the DigiPen Graphics Library (DGL).
	DGL_Texture* texture;


};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------





/*----------------------------------------------------------------------------*/

