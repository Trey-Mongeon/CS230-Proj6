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

Entity::Entity()
{
	isDestroyed = false;
	memset(name, 0, 32);
}

Entity::Entity(const Entity* other)
{
	isDestroyed = other->isDestroyed;
	memcpy(name, other->name, 32);
	for (int i = 0; i < other->components.size(); ++i)
	{
		Add(other->components[i]->Clone());
	}
}

Entity::~Entity()
{
	for (int i = 0; i < components.size(); ++i)
	{
		delete components[i];
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
bool Entity::IsNamed(const char* inName) const
{
	if (strcmp(name, inName) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// Check whether an Entity has been flagged for destruction.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return the value in the "isDestroyed" flag,
//		else return false.
bool Entity::IsDestroyed() const
{
	return isDestroyed;
}


// Flag an Entity for destruction.
// (Note: This is to avoid Entities being destroyed while they are being processed.)
// Params:
//	 entity = Pointer to the Entity to be flagged for destruction.
// Returns:
//	 If 'entity' is valid,
//	   then set the 'isDestroyed' flag,
//	   else do nothing.
void Entity::Destroy()
{
	isDestroyed = true;
}


// Read (and construct) the components associated with a entity.
// [NOTE: See project instructions for implementation instructions.]
// Params:
//	 entity = Pointer to the Entity.
//	 stream = The data stream used for reading.
void Entity::Read(Stream stream)
{
	if (stream)
	{
		strcpy_s(name, _countof(name), StreamReadToken(stream));
		while (true)
		{
			const char* token = StreamReadToken(stream);
			if (strstr(token, "Transform"))
			{
				Transform* transformPtr = new Transform;
				transformPtr->Read(stream);
				Add(transformPtr);
			}
			else if (strstr(token, "Physics"))
			{
				Physics* physicsPtr = new Physics;
				physicsPtr->Read(stream);
				Add(physicsPtr);
			}
			else if (strstr(token, "Sprite"))
			{
				Sprite* spritePtr = new Sprite;
				spritePtr->Read(stream);
				Add(spritePtr);
			}
			else if (strstr(token, "Animation"))
			{
				Animation* animationPtr = new Animation;
				animationPtr->Read(stream);
				Add(animationPtr);
			}
			else if(token[0] == '\0')
			{ 
				break;
			}
		}
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
void Entity::SetName(const char* inName)
{
	if (inName)
	{
		strcpy_s(name, _countof(name), inName);
	}
}

// Get the Entity's name.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the Entity's name,
//		else return NULL.
const char* Entity::GetName() const
{
	return name;
}

// Update any components attached to the Entity.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
//	 dt = Change in time (in seconds) since the last game loop.
void Entity::Update(float dt)
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Update(dt);
	}

}

// Render any visible components attached to the Entity.
// (Hint: You will need to call SpriteRender(), passing the Sprite and Transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
void Entity::Render()
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Render();
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

