// @file: Scene.cpp
//
// @brief: Cpp file for the Scene class. All entities are part of a Scene.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-29

#include "stdafx.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"

/**
 * @brief Scene constructor generates a random GUID & UID.
 */
Scene::Scene()
{
	UUID _guid;
	// Create a new GUID
	CreateUUID(&_guid);

	guid = GUIDTostring(_guid);
	uid = GetHashCode(guid.c_str());
}

/**
 * @brief Constructor for Scene which generates a random GUID & UID.
 * 
 * @param _guid GUID of the scene.
 */
Scene::Scene(std::string _guid)
{
	// Use the GUID passed to it
	uid = GetHashCode(guid.c_str());
	guid = _guid;
}

/**
 * @brief Initialize all the entities of this scene.
 */
void Scene::Initialize()
{
	// Initialize happens before first PreUpdate. So it must be called on entitiesToBeAdded
	for (Entity* entity : entitiesToBeAdded)
	{
		entity->Initialize();
	}
}

/**
 * @brief Load Scene data passed in a JSON.
 *
 * @param sceneJSON Scene data JSON.
 */
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
	if (sceneData.hasKey("IsEnabled"))
	{
		isEnabled = sceneData["IsEnabled"].ToBool();
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
	LOG(name << " scene - " << entitiesToBeAdded.size() << " entites loaded.")
}

/**
 * @brief Load the to-be-added entities.
 */
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

/**
 * @brief Update all the active entities.
 */
void Scene::Update()
{
	for (Entity* entity : entities)
	{
		if (entity->IsActive() && !SceneManager::Get().sceneChanged)
		{
			entity->Update();
		}
	}
}

/**
 * @brief Remove the to-be-destroyed entities.
 */
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
		if (entity->IsPartOfObjectPool())
			LOG('yo?')
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

json::JSON Scene::Save()
{
	json::JSON sceneJSON;
	sceneJSON["Name"] = name;
	sceneJSON["GUID"] = guid;
	sceneJSON["IsEnabled"] = true;
	
	json::JSON entitiesJSON = json::JSON::Array();
	for (Entity* entity : entities)
	{
		if (entity->IsActive() && !SceneManager::Get().sceneChanged)
		{
			entitiesJSON.append(entity->Save());
		}
	}
	sceneJSON["Entities"] = entitiesJSON;
	return sceneJSON;
}

/**
 * @brief Destory all the entities.
 */
void Scene::Destroy()
{
	for (Entity* entity : entities)
	{
		entity->Destroy();
		delete entity;
	}
	entities.clear();
}

/**
 * @brief Create a new entity. Scene automatically keeps track of this entity.
 *
 * @return Pointer to the created entity.
 */
Entity* Scene::CreateEntity()
{
	Entity* entity = new Entity();
	// The scene that creates an entity has its ownership
	entitiesToBeAdded.push_back(entity);
	return entity;
}

/**
 * @brief Creates an entity that is not part of a scene *yet*
 *
 * @return Pointer to the created entity.
 */
Entity* Scene::CreateDanglingEntity(bool forObjectPool)
{
	Entity* entity = new Entity();
	entity->isPartOfObjectPool = forObjectPool;
	entity->isIdleInObjectPool = forObjectPool;
	return entity;
}

void Scene::AddDanglingEntity(Entity* entity)
{
	entitiesToBeAdded.push_back(entity);
}

/**
 * @brief Find an entity in the scene.
 *
 * @param entityGUID GUID of the entity.
 * @return Pointer to the found entity.
 */
Entity* Scene::FindEntity(std::string& entityGUID)
{
	STRCODE entityId = GetHashCode(entityGUID.c_str());
	return FindEntity(entityId);
}

/**
 * @brief Find an entity in the scene.
 *
 * @param entityId UID of the entity.
 * @return Pointer to the found entity.
 */
Entity* Scene::FindEntity(STRCODE entityId)
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

/**
 * @brief Search an entity by name in the scene.
 *
 * @param entityName Name of the entity.
 * @return List of pointers to the matched entities.
 */
std::list<Entity*> Scene::FindEntityByName(std::string& entityName)
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

/**
 * @brief Lookup entities with a certain component.
 *
 * @param componentName Name of a class which inherits from Component.
 * @return List of pointers to the found entities.
 */
std::list<Entity*> Scene::FindEntityWithComponent(std::string& componentName)
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

/**
 * @brief Remove an entity from the Scene.
 *
 * @param entityGUID GUID of the entity.
 * @return Boolean representing if the entity got removed successfully.
 */
bool Scene::RemoveEntity(std::string& entityGUID)
{
	STRCODE entityId = GetHashCode(entityGUID.c_str());
	return RemoveEntity(entityId);
}

/**
 * @brief Remove an entity from the Scene.
 *
 * @param entityId UID of the entity.
 * @return Boolean representing if the entity got removed successfully.
 */
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
