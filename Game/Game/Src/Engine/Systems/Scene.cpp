// @file: Scene.cpp
//
// @brief: Cpp file for the Scene class. All entities are part of a Scene.

#include "stdafx.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Components/Entity.h"
#include "Engine/Core/Logger.h"
#include "Engine/Pools/EntityPool.h"

Scene::Scene()
{
	UUID _guid;
	// Create a new GUID
	CreateUUID(&_guid);

	guid = GUIDTostring(_guid);
	uid = GetHashCode(guid.c_str());
}

Scene::Scene(std::string& _guid)
{
	// Use the GUID passed to it
	uid = GetHashCode(guid.c_str());
	guid = _guid;
}

void Scene::Load()
{
	if (LoadSceneFunc != nullptr)
	{
		LoadSceneFunc(this);
	}
}

void Scene::Initialize()
{
	// Initialize happens before first PreUpdate. So it must be called on entitiesToBeAdded
	for (Entity* entity : entitiesToBeAdded)
	{
		entity->Initialize();
	}
	std::string logMsg = "Scene (name=" + name + ", GUID=" + guid + ") initialized with " + std::to_string(entitiesToBeAdded.size()) + " entities.";
	Logger::Get().Log(logMsg);
}

void Scene::PreUpdate()
{
	for (Entity* entity : entitiesToBeAdded)
	{
		entities.push_back(entity);
	}
	entitiesToBeAdded.clear();

	// Call pre update on all entites
	// Useful for adding any components scheduled to be added
	for (Entity* entity : entities)
	{
		if (entity->IsActive())
		{
			entity->PreUpdate();
		}
	}
}

void Scene::Update(float deltaTime)
{
	for (Entity* entity : entities)
	{
		if (entity->IsActive())
		{
			entity->Update(deltaTime);
		}
	}
}

void Scene::PostUpdate()
{
	// Call post update on all entites
	// Useful for deleting any components scheduled to be deleted
	for (Entity* entity : entities)
	{
		if (entity->IsActive())
		{
			entity->PostUpdate();
		}
	}

	for (Entity* entity : entitiesToUntrack)
	{
		entities.remove(entity);
	}
	entitiesToUntrack.clear();

	if (_reloadScene)
	{
		_reloadScene = false;
		// Destroy the scene
		Destroy();

		// Reload it
		Load();

		// Scene reload happens mid-game. Engine won't initialize it
		Initialize();
	}
}

void Scene::Destroy()
{
	// Entities will get removed in the EntityPool
	// No need of removing them here, just mark them as free
	for (Entity* entity : entities)
	{
		entity->sourcePool->MarkObjectAsFree(static_cast<Object*>(entity));
	}
	entities.clear();
	// Ensure nothing is scheduled to be added or removed
	for (Entity* entity : entitiesToBeAdded)
	{
		entity->sourcePool->MarkObjectAsFree(static_cast<Object*>(entity));
	}
	entitiesToBeAdded.clear();
	entitiesToUntrack.clear();
}

void Scene::ReloadScene()
{
	// Schedule the scene to be reloaded on subsequent post update
	_reloadScene = true;
}

Entity* Scene::CreateEntity(std::vector<ComponentType>& components)
{
	Entity* entity = SceneManager::Get().GetNewEntity(components);
	entitiesToBeAdded.push_back(entity);
	return entity;
}

void Scene::AddDanglingEntity(Entity* entity)
{
	entitiesToBeAdded.push_back(entity);
}

Entity* Scene::FindEntity(const std::string& entityGUID) const
{
	return FindEntity(GetHashCode(entityGUID.c_str()));
}

Entity* Scene::FindEntity(STRCODE entityId) const
{
	for (Entity* entity : entities)
	{
		if (entity->GetUid() == entityId)
		{
			return entity;
		}
	}
	// Maybe the entity is yet to be added
	for (Entity* entity : entitiesToBeAdded)
	{
		if (entity->GetUid() == entityId)
		{
			return entity;
		}
	}
	return nullptr;
}

std::list<Entity*> Scene::FindEntityByName(const std::string& entityName) const
{
	std::list<Entity*> foundEntities;
	for (Entity* entity : entities)
	{
		if (entity->GetName() == entityName)
		{
			foundEntities.push_back(entity);
		}
	}
	// Maybe the entity is yet to be added
	for (Entity* entity : entitiesToBeAdded)
	{
		if (entity->GetName() == entityName)
		{
			foundEntities.push_back(entity);
		}
	}
	return foundEntities;
}

std::list<Entity*> Scene::FindEntityWithComponent(ComponentType componentType) const
{
	std::list<Entity*> foundEntities;
	for (Entity* entity : entities)
	{
		if (entity->GetComponent(componentType))
		{
			foundEntities.push_back(entity);
		}
	}
	// Maybe the entity is yet to be added
	for (Entity* entity : entitiesToBeAdded)
	{
		if (entity->GetComponent(componentType))
		{
			foundEntities.push_back(entity);
		}
	}
	return foundEntities;
}

void Scene::RemoveEntity(Entity* entity)
{
	entity->sourcePool->MarkObjectAsFree(entity);
}

void Scene::RemoveEntity(std::string& entityGUID)
{
	RemoveEntity(GetHashCode(entityGUID.c_str()));
}

void Scene::RemoveEntity(STRCODE entityId)
{
	for (Entity* entity : entities)
	{
		if (entity->GetUid() == entityId)
		{
			RemoveEntity(entity);
		}
	}
}

void Scene::UntrackEntity(Entity* entity)
{
	entitiesToUntrack.push_back(entity);
}
