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

void Scene::Initialize()
{
	// Initialize happens before first PreUpdate. So it must be called on entitiesToBeAdded
	for (Entity* entity : entitiesToBeAdded)
	{
		entity->Initialize();
	}
}

void Scene::Load(json::JSON& sceneJSON)
{
	THROW_RUNTIME_ERROR(!sceneJSON.hasKey("Scene"), "Scene JSON must contain scene info.");

	json::JSON sceneData = sceneJSON["Scene"];
	if (sceneData.hasKey("Name"))
	{
		name = sceneData["Name"].ToString();
	}
	// If GUID exists, it overwrites the guid & uid populated in Scene constructor
	if (sceneData.hasKey("GUID"))
	{
		guid = sceneData["GUID"].ToString();
		uid = GetHashCode(guid.c_str());
	}

	// Load the entities
	if (sceneData.hasKey("Entities"))
	{
		json::JSON entitiesJSON = sceneData["Entities"];
		for (json::JSON& entityJSON : entitiesJSON.ArrayRange())
		{
			Entity* entity = CreateEntity();
			entity->Load(entityJSON);
		}
	}
	
	std::string logMsg = "Scene (name=" + name + ", GUID=" + guid + ") loaded with " + std::to_string(entitiesToBeAdded.size()) + " entities.";
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

	for (Entity* entity : entitiesToDestroy)
	{
		entity->Destroy();
		delete entity;
		entities.remove(entity);
	}
	entitiesToDestroy.clear();

	for (Entity* entity : entitiesToUntrack)
	{
		entities.remove(entity);
	}
	entitiesToUntrack.clear();
}

void Scene::Destroy()
{
	for (Entity* entity : entities)
	{
		entity->Destroy();
		delete entity;
	}
	entities.clear();
}

Entity* Scene::CreateEntity()
{
	// Get new entity from entity pool
	Entity* entity = static_cast<Entity*>(EntityPool::Get().GetFreeObject());
	entitiesToBeAdded.push_back(entity);
	return entity;
}

Entity* Scene::CreateDanglingEntity(bool forObjectPool) const
{
	Entity* entity = new Entity();
	entity->SetPartOfObjectPool();
	entity->MarkFreeInObjectPool();
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

std::list<Entity*> Scene::FindEntityWithComponent(const std::string& componentName) const
{
	std::list<Entity*> foundEntities;
	for (Entity* entity : entities)
	{
		if (entity->GetComponent(componentName))
		{
			foundEntities.push_back(entity);
		}
	}
	// Maybe the entity is yet to be added
	for (Entity* entity : entitiesToBeAdded)
	{
		if (entity->GetComponent(componentName))
		{
			foundEntities.push_back(entity);
		}
	}
	return foundEntities;
}

bool Scene::RemoveEntity(std::string& entityGUID)
{
	return RemoveEntity(GetHashCode(entityGUID.c_str()));
}

bool Scene::RemoveEntity(STRCODE entityId)
{
	for (Entity* entity : entities)
	{
		if (entity->GetUid() == entityId)
		{
			entitiesToDestroy.push_back(entity);
			return true;
		}
	}
	return false;
}

void Scene::UntrackEntity(Entity* entity)
{
	entitiesToUntrack.push_back(entity);
}
