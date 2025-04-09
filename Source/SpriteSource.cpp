//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"
#include "DGL.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

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


SpriteSource::SpriteSource()
{
	numCols = 1;
	numRows = 1;
	texture = NULL;
	memset(name, 0, 32);
}


SpriteSource::~SpriteSource()
{
	DGL_Graphics_FreeTexture(&texture);
}


// Load a texture from a file (may be an Col x Row sprite sheet).
// (NOTE: The folder path, "Assets/" should be prepended to the texture name before
//    opening the file.  The function sprintf_s can be used for this purpose.
//    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sprintf-s-sprintf-s-l-swprintf-s-swprintf-s-l)
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 textureName = The name of the texture to be loaded.
void SpriteSource::LoadTexture(int inNumCols, int inNumRows, const char* textureName)
{
	char newFileName[256];
	char assetStr[8] = { "Assets/" };

	sprintf_s(newFileName, _countof(newFileName), "%s%s", &assetStr, textureName);

	numCols = inNumCols;
	numRows = inNumRows;

	texture = DGL_Graphics_LoadTexture(newFileName);
	
}


// Read the properties of a SpriteSource object from a file.
// 1: Read a token from the stream.
// 2: If the token read was "SpriteSource",
//	  a: Read a token from the stream and store it in the name variable.
//       (Hint: use strcpy_s for this purpose.)
//    b: Read the numCols and numRows values from the stream.
//	  c: Read a token from the stream. This is the path name of a texture.
//	  d: Call DGL_Graphics_LoadTexture and store the texture resource.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 stream = Pointer to the data stream used for reading.
void SpriteSource::Read(Stream stream)
{
	const char* token = StreamReadToken(stream);
	if (strcmp(token, "SpriteSource") == 0)
	{
		const char* inName = StreamReadToken(stream);
		strcpy_s(name, sizeof(name), inName);
		numCols = StreamReadInt(stream);
		numRows = StreamReadInt(stream);
		const char* filePath = StreamReadToken(stream);
		texture = DGL_Graphics_LoadTexture(filePath);
	}
}


// Determines if a SpriteSource has the specified name.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the SpriteSource and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool SpriteSource::IsNamed(const char* inName) const
{
	if (inName)
	{
		if (strcmp(name, inName) == 0)
		{
			return true;
		}
	}
	return false;
}


// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
// Returns:
//	 If the SpriteSource pointer is valid,
//		then return the calculated frame count (numCols * numRows),
//		else return 0.
unsigned SpriteSource::GetFrameCount() const
{
	return numCols * numRows;
}


// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a float to contain the U coordinate. 
//   v = Pointer to a float to contain the V coordinate. 
void SpriteSource::GetUV(unsigned int frameIndex, float* u, float* v) const
{
	float uSize = 1.0f / numCols;
	float vSize = 1.0f / numRows;

	*u = uSize * (frameIndex % numCols);
	*v = vSize * (frameIndex / numCols);
}


// Sets a SpriteSource texture for rendering.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
void SpriteSource::SetTexture() const
{
	DGL_Graphics_SetTexture(texture);
}


// Sets the texture UV offsets for rendering.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
void SpriteSource::SetTextureOffset(unsigned frameIndex) const
{
	DGL_Vec2 offsetVector;
	if (&offsetVector)
	{
		GetUV(frameIndex, &offsetVector.x, &offsetVector.y);
		DGL_Graphics_SetCB_TextureOffset(&offsetVector);
	}
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

