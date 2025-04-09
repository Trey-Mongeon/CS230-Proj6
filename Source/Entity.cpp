//------------------------------------------------------------------------------
//
// File Name:	Entity.cpp
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Entity.h"
#include "sprite.h"
#include "physics.h"
#include "Transform.h"
#include "Stream.h"
#include "string.h"
#include "Animation.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
typedef struct Entity
{
	// The name of the entity.
	// A buffer is used to allow each entity to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	char name[32];

	// Pointer to an attached physics component.
	Physics* physics;

	// Pointer to an attached sprite component.
	Sprite* sprite;

	// Pointer to an attached transform component.
	Transform* transform;

	Animation* animation;

	bool isDestroyed;

} Entity;
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
// Dynamically allocate a new Entity.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Entity* EntityCreate(void)
{
	Entity* entityPtr = new Entity;
	if (entityPtr)
	{
		return entityPtr;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with an Entity.
// (NOTE: All attached components must be freed using the corresponding Free() functions.)
// (NOTE: The Entity pointer must be set to NULL.)
// Params:
//	 entity = Pointer to the Entity pointer.
void EntityFree(Entity** entity)
{
	if (entity && *entity)
	{
		SpriteFree(&(*entity)->sprite);
		PhysicsFree(&(*entity)->physics);
		TransformFree(&(*entity)->transform);
		AnimationFree(&(*entity)->animation);
		free(*entity);

   		*entity = NULL;
	}
}


// Compare the Entity's name with the specified name.
// Params:
//	 entity = Pointer to the Entity.
//   name = Pointer to the name to be checked.
// Returns:
//	 If the Entity pointer is valid and the two names match,
//		then return true,
//		else return false.
bool EntityIsNamed(const Entity* entity, const char* name)
{
	if (entity) // instruction comments dont say to check name ptr but do in other IsNamed functions
	{
		if (strcmp(entity->name, name) == 0)
		{
			return true;
		}
	}
	return false;
}


// Check whether an Entity has been flagged for destruction.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return the value in the "isDestroyed" flag,
//		else return false.
bool EntityIsDestroyed(const Entity* entity)
{
	if (entity)
	{
		return entity->isDestroyed;
	}
	return false;
}


// Flag an Entity for destruction.
// (Note: This is to avoid Entities being destroyed while they are being processed.)
// Params:
//	 entity = Pointer to the Entity to be flagged for destruction.
// Returns:
//	 If 'entity' is valid,
//	   then set the 'isDestroyed' flag,
//	   else do nothing.
void EntityDestroy(Entity* entity)
{
	if (entity)
	{
		entity->isDestroyed = true;
	}
}





// Dynamically allocate a clone of an existing Entity.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// (WARNING: You should use the EntityAdd* functions when attaching cloned
//    components to the cloned Entity.  This will ensure that the 'parent'
//    variable is set properly.)
// Params:
//	 other = Pointer to the Entity to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned Entity,
//	   else return NULL.
Entity* EntityClone(const Entity* other)
{
	if (other)
	{
		Entity* entity = new Entity;

		if (entity)
		{
			*entity = *other;
			EntityAddAnimation(entity, AnimationClone(other->animation));
			EntityAddPhysics(entity, PhysicsClone(other->physics));
			EntityAddSprite(entity, SpriteClone(other->sprite));
			EntityAddTransform(entity, TransformClone(other->transform));
		}
		return entity;
	}
	return NULL;
}


// Read (and construct) the components associated with a entity.
// [NOTE: See project instructions for implementation instructions.]
// Params:
//	 entity = Pointer to the Entity.
//	 stream = The data stream used for reading.
void EntityRead(Entity* entity, Stream stream)
{
	if (entity && stream)
	{
		strcpy_s(entity->name, _countof(entity->name), StreamReadToken(stream));
		while (true)
		{
			const char* token = StreamReadToken(stream);
			if (strstr(token, "Transform"))
			{
				Transform* transformPtr = TransformCreate();
				TransformRead(transformPtr, stream);
				entity->transform = transformPtr;
			}
			else if (strstr(token, "Physics"))
			{
				Physics* physicsPtr = PhysicsCreate();
				PhysicsRead(physicsPtr, stream);
				entity->physics = physicsPtr;
			}
			else if (strstr(token, "Sprite"))
			{
				Sprite* spritePtr = SpriteCreate();           /// PLACES WHERE CREATE IS CALLED NEW IS CALLED NOW - FREE IS DELETE
				SpriteRead(spritePtr, stream);
				entity->sprite = spritePtr;
			}
			else if (strstr(token, "Animation"))
			{
				Animation* animationPtr = AnimationCreate();
				AnimationRead(animationPtr, stream);
				EntityAddAnimation(entity, animationPtr);
			}
			else if(token[0] == '\0')
			{ 
				break;
			}
		}
	}
}



// Attach an Animation component to an Entity.
// (NOTE: This function must set the animation component's parent pointer by
//	  calling the AnimationSetParent() function.)
// Params:
//	 entity = Pointer to the Entity.
//   animation = Pointer to the Animation component to be attached.
void EntityAddAnimation(Entity* entity, Animation* inAnimation)
{
	if (entity && inAnimation)
	{
		entity->animation = inAnimation;
		entity->animation->SetParent(entity); // this good, do this for others
	}
}


// Get the Animation component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Animation component,
//		else return NULL.
Animation* EntityGetAnimation(const Entity* entity)
{
	if (entity)
	{
		return entity->animation;
	}
	else
	{
		return NULL;
	}
}


// Attach a Physics component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   physics = Pointer to the Physics component to be attached.
void EntityAddPhysics(Entity* entity, Physics* physics)
{
	if (entity && physics)
	{
	entity->physics = physics;
	}
}

// Attach a sprite component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   sprite = Pointer to the Sprite component to be attached.
void EntityAddSprite(Entity* entity, Sprite* sprite)
{
	if (entity && sprite)
	{
	entity->sprite = sprite;
	}
}

// Attach a transform component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   transform = Pointer to the Transform component to be attached.
void EntityAddTransform(Entity* entity, Transform* transform)
{
	if (entity && transform)
	{
	entity->transform = transform;
	}
}

// Set the Entity's name.
// [NOTE: Verify that both pointers are valid before setting the name.]
// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
//	 calling this function!  Instead, use the _countof() macro to get the
//	 size of the "name" array.]
// Params:
//	 entity = Pointer to the Entity.
//	 name = Pointer to the Entity's new name.
void EntitySetName(Entity* entity, const char* name)
{
	if (entity && name)
	{
		strcpy_s(entity->name, _countof(entity->name), name);
	}
}

// Get the Entity's name.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the Entity's name,
//		else return NULL.
const char* EntityGetName(const Entity* entity)
{
	if (entity)
	{
		return entity->name;
	}
	else
	{
		return NULL;
	}
}

// Get the Physics component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
Physics* EntityGetPhysics(const Entity* entity)
{
	if (entity)
	{
		return entity->physics;
	}
	else
	{
		return NULL;
	}
}

// Get the Sprite component attached to a Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Sprite component,
//		else return NULL.
Sprite* EntityGetSprite(const Entity* entity)
{
	if (entity)
	{
		return entity->sprite;
	}
	else
	{
		return NULL;
	}
}

// Get the Transform component attached to a Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Transform component,
//		else return NULL.
Transform* EntityGetTransform(const Entity* entity)
{
	if (entity)
	{
		return entity->transform;
	}
	else
	{
		return NULL;
	}
}

// Update any components attached to the Entity.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
//	 dt = Change in time (in seconds) since the last game loop.
void EntityUpdate(Entity* entity, float dt)
{
	if (entity)
	{
		if (entity->animation)
		{
			AnimationUpdate(entity->animation, dt);
		}
		if (entity->transform && entity->physics)
		{
			PhysicsUpdate(entity->physics, entity->transform, dt);
		}
	}
}

// Render any visible components attached to the Entity.
// (Hint: You will need to call SpriteRender(), passing the Sprite and Transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
void EntityRender(Entity* entity)
{
	if (entity)
	{
 		SpriteRender(entity->sprite, entity->transform);
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

