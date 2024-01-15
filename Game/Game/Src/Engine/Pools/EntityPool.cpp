// @file: EntityPool.h
//
// @brief: Header file for EntityPool, a class responsible which implements object pooling for entities.

#include "stdafx.h"

#include "Engine/Pools/EntityPool.h"
#include "Engine/Core/Object.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Renderable.h"
#include "Engine/Components/Collider.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Systems/CollisionSystem.h"

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
	// SetupObject must be called right after creating the entity for most cache coherence.
	SetupObject(obj);
	return obj;
}

void EntityPool::SetupObject(Object* object)
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

	// Remove renderable components from RenderSystem
	// - Mesh Renderer
	Component* component = entity->GetComponent(MeshRendererC);
	if (component != nullptr)
		RenderSystem::Get().RemoveRenderable(static_cast<Renderable*>(component));
	// - Sprite
	component = entity->GetComponent(SpriteC);
	if (component != nullptr)
		RenderSystem::Get().RemoveRenderable(static_cast<Renderable*>(component));
	// - BoxColliderC
	component = entity->GetComponent(BoxColliderC);
	if (component != nullptr)
	{
		RenderSystem::Get().RemoveRenderable(static_cast<Renderable*>(component));
		// Remove collider component from CollisionSystem
		CollisionSystem::Get().RemoveCollider(static_cast<Collider*>(component));
	}
}

void EntityPool::InitializeObject(Object* object)
{
	if (!object->IsEntity())
		return;

	Entity* entity = static_cast<Entity*>(object);

	// Add renderable components to RenderSystem
	// - Mesh Renderer
	Component* component = entity->GetComponent(MeshRendererC);
	if (component != nullptr)
		RenderSystem::Get().AddRenderable(static_cast<Renderable*>(component));
	// - Sprite
	component = entity->GetComponent(SpriteC);
	if (component != nullptr)
		RenderSystem::Get().AddRenderable(static_cast<Renderable*>(component));
	// - Box Collider
	component = entity->GetComponent(BoxColliderC);
	if (component != nullptr)
	{
		RenderSystem::Get().AddRenderable(static_cast<Renderable*>(component));
		// Add collider component to CollisionSystem
		CollisionSystem::Get().AddCollider(static_cast<Collider*>(component));
	}
}
