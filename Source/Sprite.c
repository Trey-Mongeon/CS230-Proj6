//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
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

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
typedef struct Sprite
{
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

} Sprite;
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
// Dynamically allocate a new Sprite component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: You must initialize the 'alpha' member variable to 1.0f.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Sprite* SpriteCreate(void)
{
	Sprite* spritePtr = calloc(1, sizeof(Sprite));

	if (spritePtr)
	{
		spritePtr->alpha = 1.0f;
		return spritePtr;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with a Sprite component.
// (NOTE: The Sprite pointer must be set to NULL.)
// Params:
//	 sprite = Pointer to the Sprite pointer.
void SpriteFree(Sprite** sprite)
{
	if (sprite)
	{
		free(*sprite);
		*sprite = NULL;
	}
}


// Dynamically allocate a clone of an existing Sprite.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Sprite* SpriteClone(const Sprite* other)
{
	if (other)
	{
		Sprite* newSprite = calloc(1, sizeof(Sprite));

		if (newSprite)
		{
			*newSprite = *other;
			return newSprite;
		}
	}
	return NULL;
}


// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = The data stream used for reading.
void SpriteRead(Sprite* sprite, Stream stream)
{
	if (sprite)
	{
		sprite->frameIndex = StreamReadInt(stream);
		sprite->alpha = StreamReadFloat(stream);
		const char* meshName = StreamReadToken(stream);
		const Mesh* builtMesh = MeshLibraryBuild(meshName);
		SpriteSetMesh(sprite, builtMesh);

		const char* sourceName = StreamReadToken(stream);
		const SpriteSource* spriteSource  = SpriteSourceLibraryBuild(sourceName);
		SpriteSetSpriteSource(sprite, spriteSource);
	}
}


// Render a Sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the Sprite component.
//   transform = Pointer to the Transform component.
void SpriteRender(const Sprite* sprite, Transform* transform)
{
   	if (sprite && sprite->mesh)
	{

		if (sprite->spriteSource)
		{
			DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);
			SpriteSourceSetTexture(sprite->spriteSource);
			SpriteSourceSetTextureOffset(sprite->spriteSource, sprite->frameIndex);

		}
		else
		{
			DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
		}
		DGL_Graphics_SetCB_Alpha(sprite->alpha);
		DGL_Color tintColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		DGL_Graphics_SetCB_TintColor(&tintColor);

		if (sprite->text == NULL)
		{
			DGL_Graphics_SetCB_TransformMatrix(TransformGetMatrix(transform));
			MeshRender(sprite->mesh);
		}
		else
		{
			Matrix2D matrix = *TransformGetMatrix(transform);
			Vector2D transformScale = *TransformGetScale(transform);
			Matrix2D translationMatrix;
			Matrix2DTranslate(&translationMatrix, transformScale.x, 0.0f);

			const char* spriteText = sprite->text;

			while (*spriteText)
			{
				int currentIndex = *spriteText - ' ';
				SpriteSourceSetTextureOffset(sprite->spriteSource, currentIndex);
				DGL_Graphics_SetCB_TransformMatrix(&matrix);
				MeshRender(sprite->mesh);
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
float SpriteGetAlpha(const Sprite* sprite)
{
	if (sprite)
	{
		return sprite->alpha;
	}
	else
	{
		return 0.0f;
	}
}


// Set a Sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: The min() and max() macros can be combined to create a clamp function.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   alpha = The Sprite's new alpha value.
void SpriteSetAlpha(Sprite* sprite, float alpha)
{
	if (sprite)
	{
	alpha = min(1.0f, alpha);
	alpha = max(0.0f, alpha);
	sprite->alpha = alpha;
	}
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
void SpriteSetFrame(Sprite* sprite, unsigned int frameIndex)
{
	if (sprite)
	{

		if (frameIndex >= 0 && frameIndex < SpriteSourceGetFrameCount(sprite->spriteSource))
		{
			sprite->frameIndex = frameIndex;

			TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
			return;
		}

	}

}


// Set the Sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   mesh = Pointer to a Mesh object.
void SpriteSetMesh(Sprite* sprite, const Mesh* mesh)
{
	if (sprite && mesh)
	{
		sprite->mesh = mesh;
	}
}


// Set a new SpriteSource for the specified Sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a Sprite and cause the Sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 spriteSource = Pointer to a SpriteSource (this pointer may be NULL).
void SpriteSetSpriteSource(Sprite* sprite, const SpriteSource* spriteSource)
{
	if (sprite && spriteSource)
	{
		sprite->spriteSource = spriteSource;
	}
}



// Assign a text string to a Sprite component.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 text = Pointer to a zero-terminated array of characters.
void SpriteSetText(Sprite* sprite, const char* text)
{
	if (sprite)
	{
		sprite->text = text;
	}
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

