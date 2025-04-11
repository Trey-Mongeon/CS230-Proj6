//------------------------------------------------------------------------------
//
// File Name:	Entity.h
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
#include <vector>
#include "Component.h"
#include <algorithm>
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Animation;
class Entity;
class Physics;
class Sprite;
class Transform;
class Component;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Entity
{

public:

	Entity();

	Entity(const Entity* other);

	~Entity();

	void Read(Stream stream);

	void Destroy();

	bool IsDestroyed() const;

	void SetName(const char* name);

	const char* GetName() const;

	bool IsNamed(const char* name) const;

	Component* Entity::BinarySearch(Component::TypeEnum type) const
	{
		size_t begin = 0;
		size_t end = components.size();

		while (begin < end)
		{
			size_t mid = (begin + end) / 2;
			if (components[mid]->Type() < type)
				begin = mid + 1;
			else
				end = mid;
		}
		if ((begin < components.size()) && (components[begin]->Type() == type))
		{
			// Return the first component found that matches the specified type.
			return components[begin];
		}
		else
		{
			// No component matches the specified type.
			return NULL;
		}
	}


// Get a component of the specified type.
// Return NULL if no such component is found.
	Component* Entity::Get(Component::TypeEnum type)
	{
		return BinarySearch(type);
	}


	// Get a component of the specified type.
// Return NULL if no such component is found.
	const Component* Entity::Get(Component::TypeEnum type) const
	{
		return BinarySearch(type);
	}


	/// Type safe method for accessing the components.
	template<typename type>
	type* GetComponent(Component::TypeEnum typeId)
	{
		return static_cast<type*>(Get(typeId));
	}

	/// Type safe method for accessing the components.
	template<typename type>
	const type* GetComponent(Component::TypeEnum typeId) const
	{
		return static_cast<const type*>(Get(typeId));
	}

	void Update(float dt);

	void Render();

	void Entity::Add(Component* component)
	{
		if (component)
		{
			// Set the component's parent.
			component->SetParent(this);

			// Add the component to the components list.
			components.push_back(component);

			// Sort the list for faster access using a binary search.
			std::sort(components.begin(), components.end(), Component::Comparator);
		}
	}


private:

	char name[32];


	bool isDestroyed;

	// Container for attached components (std::vector).
	std::vector<Component*> components;

};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------



