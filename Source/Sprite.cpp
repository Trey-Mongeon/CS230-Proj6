//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Sprite.h"
#include "Stream.h"
#include "Transform.h"
#include "DGL.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Trace.h"
#include "Matrix2D.h"
#include "MeshLibrary.h"
#include "SpriteSourceLibrary.h"
#include "Entity.h"

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


Sprite::Sprite()
{
	alpha = 1.0f;
	frameIndex = 0;
	mType = cSprite;
	spriteSource = NULL;
	mesh = NULL;
	text = NULL;
}


Sprite::Sprite(const Sprite& other)
{
	mType = other.mType;

	mParent = NULL;

	frameIndex = other.frameIndex;

	alpha = other.alpha;

	spriteSource = other.spriteSource;

	mesh = other.mesh;

	text = other.text;
}


Sprite::~Sprite()
{

}


// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = The data stream used for reading.
void Sprite::Read(Stream stream)
{
	frameIndex = StreamReadInt(stream);
	alpha = StreamReadFloat(stream);
	const char* meshName = StreamReadToken(stream);
	mesh = MeshLibraryBuild(meshName);

	const char* sourceName = StreamReadToken(stream);
	spriteSource = SpriteSourceLibraryBuild(sourceName);
}


// Render a Sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the Sprite component.
//   transform = Pointer to the Transform component.
void Sprite::Render() const
{
   	if (mesh)
	{
		Transform* transform = GetParent()->GetComponent<Transform>(cTransform);
		if (spriteSource)
		{
			DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);
			SpriteSourceSetTexture(spriteSource);
			SpriteSourceSetTextureOffset(spriteSource, frameIndex);

		}
		else
		{
			DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
		}
		DGL_Graphics_SetCB_Alpha(alpha);
		DGL_Color tintColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		DGL_Graphics_SetCB_TintColor(&tintColor);

		if (text == NULL)
		{
			DGL_Graphics_SetCB_TransformMatrix(transform->GetMatrix());
			MeshRender(mesh);
		}
		else
		{
			Matrix2D matrix = *transform->GetMatrix();
			Vector2D transformScale = *transform->GetScale();
			Matrix2D translationMatrix;
			Matrix2DTranslate(&translationMatrix, transformScale.x, 0.0f);

			const char* spriteText = text;

			while (*spriteText)
			{
				int currentIndex = *spriteText - ' ';
				SpriteSourceSetTextureOffset(spriteSource, currentIndex);
				DGL_Graphics_SetCB_TransformMatrix(&matrix);
				MeshRender(mesh);
				++spriteText;
				Matrix2DConcat(&matrix, &translationMatrix, &matrix);
			}
		}

	}
}


// Get a Sprite's alpha value.
// Params:
//	 sprite = Pointer to the Sprite component.
// Returns:
//	 If the pointer is valid,
//		then return the Sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float Sprite::GetAlpha()
{
	return alpha;
}


// Set a Sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: The min() and max() macros can be combined to create a clamp function.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   alpha = The Sprite's new alpha value.
void Sprite::SetAlpha(float inAlpha)
{
	
	inAlpha = min(1.0f, inAlpha);
	inAlpha = max(0.0f, inAlpha);
	alpha = inAlpha;
}


// Set a Sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count - 1] before changing the Sprite's frame index!)
// Params:
//	 sprite = Pointer to the Sprite component.
//   frameIndex = New frame index for the Sprite (0 .. frame count - 1).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
void Sprite::SetFrame(unsigned int inFrameIndex)
{
	if (frameIndex >= 0 && frameIndex < SpriteSourceGetFrameCount(spriteSource))
	{
		frameIndex = inFrameIndex;

		TraceMessage("SpriteSetFrame: frame index = %d", inFrameIndex);
		return;
	}

}


// Set the Sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   mesh = Pointer to a Mesh object.
void Sprite::SetMesh(const Mesh* inMesh)
{

	mesh = inMesh;
}


// Set a new SpriteSource for the specified Sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a Sprite and cause the Sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 spriteSource = Pointer to a SpriteSource (this pointer may be NULL).
void Sprite::SetSpriteSource(const SpriteSource* inSpriteSource)
{
	spriteSource = inSpriteSource;
}



// Assign a text string to a Sprite component.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 text = Pointer to a zero-terminated array of characters.
void Sprite::SetText(const char* inText)
{
	text = inText;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

