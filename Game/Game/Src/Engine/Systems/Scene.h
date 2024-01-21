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
	// Any entity which is part of this becomes dangling
	// Useful in object pooling
	std::list<Entity*> entitiesToUntrack;

	// Function to load the initial data of the scene
	std::function<void(Scene*)> LoadSceneFunc = nullptr;
	bool _loadScene = false;

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

	// Destructor is protected so that only SceneManager can delete a scene.
	~Scene() = default;

	/**
	 * @brief Load the entities.
	 */
	void Load();

	/**
	 * @brief Initialize all the entities of this scene.
	 */
	void Initialize();

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
	 * @param components Vector containing components that are part of this entity.
	 * Useful in determining appropriate entity pool.
	 * @return Pointer to the created entity.
	 */
	Entity* CreateEntity(std::vector<ComponentType>& components);
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
	 * @param componentType Type of the component
	 * @return List of pointers to the found entities.
	 */
	std::list<Entity*> FindEntityWithComponent(ComponentType componentType) const;


	/**
	 * @brief Remove an entity from the active scene.
	 * It does not actually free up the memory of the entity.
	 * It simply returns the entity back to the object pool and marks it as free.
	 * 
	 * @param entity Pointer to the entity
	 */
	void RemoveEntity(Entity* entity);
	void RemoveEntity(STRCODE entityId);
	void RemoveEntity(std::string& entityGUID);

	/**
	 * @brief Remove an entity from the Scene without actually deleting it.
	 *
	 * @param entity Pointer to the entity.
	 */
	void UntrackEntity(Entity* entity);

	// ----------------------- Getters & Setters -----------------------------------

	const std::string& GetGUID() const { return guid; }
	const STRCODE GetUID() const { return uid; }
	const std::string& GetName() const { return name; }
	void SetName(const std::string& n) { name = n; }
	void SetLoadSceneFunc(std::function<void(Scene*)> foo) { LoadSceneFunc = foo; }

	friend class SceneManager;
};

#endif // !_SCENE_H_
