//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityContainer.h"
#include "Entity.h"
#include "Collider.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct EntityContainer
{
	// This variable is not required but could be used for tracking the number
	//   of Entities currently in the list.  Instructions on how to do this
	//   are included in the function headers.
	unsigned entityCount;

	// This variable is not required but could be used for different purposes.
	// - For storing the maximum size of the container.
	// - For tracking peak usage of the container, used for testing purposes.
	unsigned entityMax;

	// This list can be a fixed-length array (minimum size of 100 entries)
	// or a dynamically sized array, such as a linked list.
	// (NOTE: The implementation details are left up to the student.  However,
	//    it is your responsiblity to ensure that memory is handled correctly.)
	Entity* entitiesArr[256];

} EntityContainer;
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

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


// Dynamically allocate a new EntityContainer.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: If your implementation requires any variables to be initialized to
//    non-zero values, then do so here.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
EntityContainer* EntityContainerCreate()
{
	EntityContainer* container = calloc(1, sizeof(EntityContainer));

	if (container)
	{
		container->entityMax = _countof(container->entitiesArr);
		return container;
	}
	else
	{
		return NULL;
	}
}


// Free all Entities in the EntityContainer.
// (NOTE: You must call EntityFree for all Entities.)
// (HINT: The container must be completely empty after this function is called.)
// (HINT: The entityCount should be 0 after all of the Entities have been freed.)
// Params:
//   entities = Pointer to the EntityContainer.
void EntityContainerFreeAll(EntityContainer* entities)
{
	if (entities)
	{
		for (unsigned int i = 0; i < entities->entityCount; ++i)
		{
			EntityFree(&(entities)->entitiesArr[i]);
		}

		entities->entityCount = 0;
	}
}

// Free the memory associated with an EntityContainer.
// (NOTE: If necessary, call EntityContainerFreeAll to free any existing Entities.)
// (NOTE: The EntityContainer pointer must be set to NULL.)
// Params:
//	 entities = Pointer to the EntityContainer pointer.
void EntityContainerFree(EntityContainer** entities)
{
	if (entities)
	{

		if (&(*entities)->entitiesArr)
		{
			EntityContainerFreeAll(*entities);
			free(*entities);
		}
		*entities = NULL;
	}
}


// Add an Entity to the EntityContainer.
// (NOTE: If the container is not full, then the Entity should be added to the list
//    and the entityCount incremented by 1.)
// Params:
//   entities = Pointer to the EntityContainer.
// Returns:
//	 If the EntityContainer pointer is valid and the Entity was added successfully,
//		then return true,
//		else return false.
bool EntityContainerAddEntity(EntityContainer* entities, Entity* entity)
{
	if (entity && entities)
	{
		if (entities->entityCount < entities->entityMax)
		{
			entities->entitiesArr[entities->entityCount] = entity;
			++entities->entityCount;

			return true;
		}
	}
	return false;
}


// Find an Entity in the EntityContainer that has a matching name.
// (HINT: Use the new function, EntityIsNamed, to compare names.)
// Params:
//   entities = Pointer to the EntityContainer.
//   entityName = The name of the Entity to be returned.
// Returns:
//	 If the EntityContainer pointer is valid and the Entity was located successfully,
//		then return a pointer to the Entity,
//		else return NULL.
Entity* EntityContainerFindByName(const EntityContainer* entities, const char* entityName)
{
	if (entities)
	{

		for (unsigned int i = 0; i < entities->entityCount; ++i)
		{
			if (EntityIsNamed(entities->entitiesArr[i], entityName))
			{
				return entities->entitiesArr[i];
			}

		}
	}
	return NULL;
}


// Determines if the EntityContainer is empty (no Entities exist).
// Params:
//   entities = Pointer to the EntityContainer.
// Returns:
//	 If the EntityContainer pointer is valid and no Entities exist,
//		then return true,
//		else return false.
bool EntityContainerIsEmpty(const EntityContainer* entities)
{
	if (entities)
	{
		if (entities->entitiesArr[0] == NULL)
		{
			return true;
		}
	}
	return false;
}


// Update all Entities in the EntityContainer.
// (HINT: You must call EntityUpdate for all Entities.)
// (NOTE: After an Entity has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the entityCount should be decremented by 1.)
// Params:
//   entities = Pointer to the EntityContainer.
//	 dt = Change in time (in seconds) since the last game loop.
void EntityContainerUpdateAll(EntityContainer* entities, float dt)
{
	if (entities)
	{
		for (unsigned int i = 0; i < entities->entityMax; ++i)
		{
			EntityUpdate(entities->entitiesArr[i], dt);

			if (EntityIsDestroyed(entities->entitiesArr[i]))
			{
   				EntityFree(&entities->entitiesArr[i]);
				--entities->entityCount;
				entities->entitiesArr[i] = (entities->entitiesArr[entities->entityCount]);
				(entities->entitiesArr[entities->entityCount]) = NULL;
				--i;
			}
		}
	}
}


// Render all Entities in the EntityContainer.
// (HINT: You must call EntityRender for all Entities.)
// Params:
//   entities = Pointer to the EntityContainer.
void EntityContainerRenderAll(const EntityContainer* entities)
{
	if (entities)
	{
		for (unsigned int i = 0; i < entities->entityCount; ++i)
		{
			EntityRender(entities->entitiesArr[i]);
		}
	}
}


// Check for collisions between all Entities in the EntityContainer.
// (HINT: Only check for collisions between Entities that contain a Collider.)
// (NOTE: There must never be more than 1 collision check between any two
//    Entities in the container.  Additionally, each Entity must never
//    check for a collision with itself.  To ensure this is the case, each time
//    an Entity with a Collider is found, check for collisions against only the
//    remaining Entities in the list (starting with the Entity following the
//    current Entity.)
// Params:
//   entities = Pointer to the EntityContainer.
void EntityContainerCheckCollisions(EntityContainer* entities)
{
	if (entities)
	{
		for (unsigned int i = 0; i < entities->entityCount; ++i)
		{
			if (EntityGetCollider(entities->entitiesArr[i]))
			{
				for (unsigned int j = i + 1; j < entities->entityCount; ++j)
				{
					if (EntityGetCollider(entities->entitiesArr[j]))
					{
						ColliderCheck(EntityGetCollider(entities->entitiesArr[i]), EntityGetCollider(entities->entitiesArr[j]));
					}
				}
			}
		}
	}
}
