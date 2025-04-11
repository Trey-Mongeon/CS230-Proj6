//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceLibrary.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSourceLibrary.h"
#include "SpriteSource.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SpriteSourceLibrary
{
	// The number of sprite sources currently in the list.
	unsigned int spriteSourceCount;

	// A list of all currently loaded sprite sources.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	SpriteSource* spriteSourceList[16];

} SpriteSourceLibrary;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static SpriteSourceLibrary sources;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Initialize the SpriteSource Library.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceLibraryInit()
{
	sources.spriteSourceCount = 0;

	for (unsigned int i = 0; i < _countof(sources.spriteSourceList); ++i)
	{
		sources.spriteSourceList[i] = 0;
	}
}


// Free all SpriteSource objects in the SpriteSource Library.
// (NOTE: You must call SpriteSourceFree() to free each SpriteSource object.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceLibraryFreeAll()
{
	for (unsigned int i = 0; i < sources.spriteSourceCount; ++i)
	{
		SpriteSourceFree(&sources.spriteSourceList[i]);
	}
	sources.spriteSourceCount = 0;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void SpriteSourceLibraryAdd(SpriteSource* spriteSource)
{
	if (spriteSource == NULL)
	{
		return;
	}
	sources.spriteSourceList[sources.spriteSourceCount] = spriteSource;
	++sources.spriteSourceCount;
}


static const SpriteSource* SpriteSourceLibraryFind(const char* sourceName)
{
	if (sourceName == NULL)
	{
		return NULL;
	}
	for (unsigned int i = 0; i < sources.spriteSourceCount; ++i)
	{
		if (SpriteSourceIsNamed(sources.spriteSourceList[i], sourceName))
		{
			return sources.spriteSourceList[i];
		}
	}
	return NULL;
}


// Create a SpriteSource and add it to the SpriteSource manager.
// 1: Call SpriteSourceLibraryFind() to determine if the sprite source already exists
// 2: If the named sprite source does not already exist,
//	  a: Use sprintf_s() to construct a path name using spriteSourceName
//	     (HINT: The correct path name should be constructed using "Data/%s.txt".)
//	  b: Call StreamOpen(), passing the path name
//    c: If the stream was opened successfully,
//		 1: Call SpriteSourceCreate() to construct a new, empty SpriteSource.
//       2: Call SpriteSourceRead() to read the SpriteSource data from the file.
//		 3: Call SpriteSourceLibraryAdd(), passing the created SpriteSource.
//       4: Close the stream.
// 3: Return the SpriteSource (already existing or just created)
// Params:
//	 SpriteSourceName = The name of the SpriteSource to be created.
// Returns:
//	 If the SpriteSource already exists or was created successfully,
//	   then return a pointer to the SpriteSource,
//	   else return NULL.
const SpriteSource* SpriteSourceLibraryBuild(const char* spriteSourceName)
{
	if (spriteSourceName == NULL || !strcmp(spriteSourceName, "None"))
	{
		return NULL;
	}

	const SpriteSource* foundSource = SpriteSourceLibraryFind(spriteSourceName);
	if (foundSource == NULL)
	{
		SpriteSource* makeSource = NULL;

		char filePath[FILENAME_MAX] = "";
		sprintf_s(filePath, _countof(filePath), "Data/%s.txt", spriteSourceName);
		Stream stream = StreamOpen(filePath);

		if (stream)
		{
			makeSource = SpriteSourceCreate();
			SpriteSourceRead(makeSource, stream);
			SpriteSourceLibraryAdd(makeSource);
			StreamClose(&stream);
			return makeSource;
		}
	}
	return foundSource;
}