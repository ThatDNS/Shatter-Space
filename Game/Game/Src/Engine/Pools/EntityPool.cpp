// @file: EntityPool.h
//
// @brief: Header file for EntityPool, a class responsible which implements object pooling for entities.

#include "stdafx.h"

#include "Engine/Pools/EntityPool.h"
#include "Engine/Core/Object.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/Entity.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

EntityPool::EntityPool(std::vector<ComponentType>& components)
{
	componentTypes = components;
	for (size_t i = 0; i < poolSize; i++)
	{
		Object* object = CreateObjectForPool();
		objects.push_back(object);

		// All are free in the beginning
		availableIndices.push(i);

		// Populate map
		objectPtrToIndex[object] = i;
	}
}

EntityPool::~EntityPool()
{
	for (Object* object : objects)
	{
		object->Destroy();
		delete object;
	}
	objects.clear();
}

Object* EntityPool::CreateObjectForPool()
{
	Object* obj = new Entity();
	// InitializeObjectForUse must be called right after creating the entity for most cache coherence.
	InitializeObjectForUse(obj);
	return obj;
}

void EntityPool::InitializeObjectForUse(Object* object)
{
	if (!object->IsEntity())
		return;

	Entity* entity = static_cast<Entity*>(object);
	for (ComponentType componentType : componentTypes)
	{
		// "Transform" component is not created dynamically. It is part of an Entity.
		// Ideally, "Transform" will never be here, but in case the game developer tris to
		// manually add Transform, this check saves the game from unintended errors.
		if (componentType == TransformC)
			continue;

		Component* component = CreateComponent(componentType);
		if (component == nullptr)
		{
			Logger::Get().Log("Entity pool tried to create null component in entity " + entity->GetName());
			continue;
		}
		entity->componentsToAdd.push_back(component);

		// All components must know about their entity
		component->entity = entity;
	}
	// Attach source pool
	entity->sourcePool = this;
}

void EntityPool::CleanUpObject(Object* object)
{
	if (!object->IsEntity())
		return;

	Entity* entity = static_cast<Entity*>(object);

	// Remove it from the scene
	Scene* scene = SceneManager::Get().GetActiveScene();
	if (scene == nullptr)
		return;

	scene->UntrackEntity(entity);

	// Remove all components
	entity->Destroy();
}
