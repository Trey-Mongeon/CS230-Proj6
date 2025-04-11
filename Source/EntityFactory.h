//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright � 2025 DigiPen (USA) Corporation.
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

class Entity;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified Entity.
// (NOTE: Use sprintf_s() to construct a path name using objectName.)
// (HINT: The correct path name should be constructed using "Data/%s.txt".)
// (HINT: See project instructions.)
// Params:
//	 entityName = The name of the Entity to be constructed.
// Returns:
//	 If the objectName is not NULL,
//	   then return a pointer to a new instance of the specified Entity,
//	   else NULL.
Entity* EntityFactoryBuild(const char * entityName);

// Free all archetype Entities.
// (Hint: If the "archetypes" container exists, then the EntityContainerFreeAll
//    function must be called.)
void EntityFactoryFreeAll();

//------------------------------------------------------------------------------



