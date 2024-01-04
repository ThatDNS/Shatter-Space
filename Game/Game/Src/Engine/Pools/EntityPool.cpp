// @file: EntityPool.h
//
// @brief: Header file for EntityPool, a class responsible which implements object pooling for entities.

#include "stdafx.h"

#include "Engine/Pools/EntityPool.h"
#include "Engine/Core/Object.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/Entity.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

EntityPool::EntityPool()
{
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
	Entity* entity = SceneManager::Get().GetActiveScene()->CreateDanglingEntity(true);
	Object* obj = static_cast<Object*>(entity);
	InitializeObjectForUse(obj);
	return obj;
}

void EntityPool::InitializeObjectForUse(Object* object)
{
	if (!object->IsEntity())
		return;

	Entity* entity = static_cast<Entity*>(object);

	// Entities in pool might not have a transform
	if (entity->transform == nullptr)
	{
		entity->transform = (Transform*)entity->CreateComponent("Transform");
		entity->isEntity = true;
	}
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
	entity->MarkFreeInObjectPool();

	// Remove all components. Even transform, to prevent unintended memory leaks
	entity->Destroy();
	// Just to double ensure that nothing ends up accessing deleted memory addr
	entity->transform = nullptr;
}
