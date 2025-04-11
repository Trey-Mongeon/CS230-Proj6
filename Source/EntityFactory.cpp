//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.cpp
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityFactory.h"
#include "Stream.h"
#include "Entity.h"
#include "Sprite.h"
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
//static EntityContainer* archetypes = NULL;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Build a single instance of the specified entity.
// (HINT: See project instructions.)
// Params:
//	 filename = The name of the file to be deserialized.
// Returns:
//	 If the filename is valid
//	   then return a pointer to a new instance of the specified entity,
//	   else NULL.
Entity* EntityFactoryBuild(const char* entityName)
{
	if (entityName)
	{
		
		char pathName[FILENAME_MAX] = "";

		sprintf_s(pathName, _countof(pathName), "Data/%s.txt", entityName);

		Stream stream = StreamOpen(pathName);

		if (stream)
		{
			const char* token = StreamReadToken(stream);

			if (!strncmp("Entity", token, _countof("Entity")))
			{
				Entity* entityPtr = new Entity;
				entityPtr->Read(stream);

				StreamClose(&stream);
				return entityPtr;
			}
		}
	}
	return NULL;
}


// Free all archetype Entities.
// (Hint: If the "archetypes" container exists, then the EntityContainerFreeAll
//    function must be called.)
void EntityFactoryFreeAll()
{
	//fard
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

