// @file: Scene.h
//
// @brief: Header file for the Scene class. All entities are part of a Scene.

#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

class Entity;

/**
 * @class Scene
 *
 * Scene class contains and manages all the entities within it.
 */
class Scene final
{
private:
	std::string name = "";
	std::string guid = "";
	STRCODE uid = 0;

	std::list<Entity*> entitiesToBeAdded;
	std::list<Entity*> entities;
	std::list<Entity*> entitiesToDestroy;

	// Any entity which is part of this becomes dangling
	// Useful in object pooling
	std::list<Entity*> entitiesToUntrack;

protected:
	/**
	 * @brief Scene constructor generates a random GUID & UID.
	 */
	Scene();

	/**
	 * @brief Constructor for Scene which generates a random GUID & UID.
	 *
	 * @param guid GUID of the scene.
	 */
	Scene(std::string& guid);
	~Scene() = default;

	/**
	 * @brief Initialize all the entities of this scene.
	 */
	void Initialize();
	/**
	 * @brief Load Scene data passed in a JSON.
	 *
	 * @param sceneJSON Scene data JSON.
	 */
	void Load(json::JSON& sceneJSON);

	/**
	 * @brief Load the to-be-added entities.
	 */
	void PreUpdate();
	/**
	 * @brief Update all the active entities.
	 */
	void Update(float);
	/**
	 * @brief Remove the to-be-destroyed entities.
	 */
	void PostUpdate();

	/**
	 * @brief Destory all the entities.
	 */
	void Destroy();

public:
	/**
	 * @brief Create a new entity in the scene.
	 *
	 * @return Pointer to the created entity.
	 */
	Entity* CreateEntity();
	/**
	 * @brief Creates an entity that is not part of a scene *yet*.
	 *
	 * @param forObjectPool Bool representing if entity is intended to be used in an object pool.
	 * @return Pointer to the created entity.
	 */
	Entity* CreateDanglingEntity(bool forObjectPool) const;
	/**
	 * @brief Add a dangling entity to the scene.
	 *
	 * @param entity Entity pointer
	 */
	void AddDanglingEntity(Entity* entity);

	/**
	 * @brief Find an entity in the scene.
	 *
	 * @param entityGUID GUID of the entity.
	 * @return Pointer to the found entity.
	 */
	Entity* FindEntity(const std::string& entityGUID) const;
	/**
	 * @brief Find an entity in the scene.
	 *
	 * @param entityId UID of the entity.
	 * @return Pointer to the found entity.
	 */
	Entity* FindEntity(STRCODE entityId) const;
	/**
	 * @brief Search an entity by name in the scene.
	 * Entities in a scene can have same name.
	 *
	 * @param entityName Name of the entity.
	 * @return List of pointers to the matched entities.
	 */
	std::list<Entity*> FindEntityByName(const std::string& entityName) const;
	/**
	 * @brief Lookup entities with a certain component.
	 *
	 * @param componentName Name of a class, which inherits from Component.
	 * @return List of pointers to the found entities.
	 */
	std::list<Entity*> FindEntityWithComponent(const std::string& componentClassName) const;

	/**
	 * @brief Remove an entity from the Scene and delete it.
	 *
	 * @param entityGUID GUID of the entity.
	 * @return Boolean representing if the entity got removed successfully.
	 */
	bool RemoveEntity(std::string& entityGUID);
	/**
	 * @brief Remove an entity from the Scene and delete it.
	 *
	 * @param entityId UID of the entity.
	 * @return Boolean representing if the entity got removed successfully.
	 */
	bool RemoveEntity(STRCODE entityId);
	/**
	 * @brief Remove an entity from the Scene without actually deleting it.
	 *
	 * @param entity Pointer to the entity.
	 */
	void UntrackEntity(Entity* entity);

	// ----------------------- Getters -----------------------------------

	const std::string& GetGUID() const { return guid; }
	const STRCODE GetUID() const { return uid; }
	const std::string& GetName() const { return name; }

	friend class SceneManager;
};

#endif // !_SCENE_H_
