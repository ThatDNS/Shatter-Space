// @file: SceneManager.h
//
// @brief: Header file for SceneManager, a singleton responsible for handling all scenes, entities, and components.
// It can keep track of multiple scenes but there can be only 1 active scene at a time.
//
// In future, this class can be easily extended to support rendering of multiple scenes at a time.
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-29

#pragma once
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

class Scene;
class Entity;

/**
 * @class SceneManager
 *
 * Scene Manager class provides an API to manage scenes and their entities.
 */
class SceneManager
{
	friend class Engine;

private:
	DECLARE_SINGLETON(SceneManager)

	const std::string DATA_FILE = "Assets/Scenes/SceneManager.json";

	Scene* activeScene = nullptr;

	// UID of the Scene to be set as active (happens in pre-update)
	// Useful when user changes the current scene
	STRCODE toBeSetAsActive = 0;

	// Keep track of file location for each Scene available (this data comes from DATA_FILE json)
	std::map <STRCODE, std::string> stringUIDToFile;

protected:
	/**
	 * @brief Load Scene Manager data from SceneManager.json file.
	 * It loads up the first scene (active scene) and keeps track of all the other scenes).
	 */
	void Load();

	/**
	 * @brief Initialize the active scene.
	 */
	void Initialize();

	/**
	 * @brief Call PreUpdate on the active scene.
	 * If active scene got changed, the actual switch happens here.
	 */
	void PreUpdate();

	/**
	 * @brief Update the active scene.
	 */
	void Update();

	/**
	 * @brief Call PostUpdate on the active scene.
	 * If active scene got changed, the current scene gets destroyed here.
	 */
	void PostUpdate();

	/**
	 * @brief Clean-up function for SceneManager.
	 * Destroys the active scene.
	 */
	void Destroy();

public:
	// ------------------------- Scene-related member functions -------------------------

	/**
	 * @brief Getter function to get the active scene.
	 *
	 * @return Pointer to the active scene.
	 */
	Scene* GetActiveScene();
	/**
	 * @brief Getter function to get the active scene.
	 *
	 * @return ID of the active scene.
	 */
	STRCODE GetActiveSceneId();

	/**
	 * @brief Setter function to set the active scene using Scene GUID.
	 *
	 * @param sceneGUID GUID of the scene to be set as active.
	 * @return Bool representing if operation was successful.
	 */
	bool SetActiveScene(const std::string& sceneGUID);
	/**
	 * @brief Setter function to set the active scene using Scene UID.
	 *
	 * @param sceneId UID of the scene to be set as active.
	 * @return Bool representing if operation was successful.
	 */
	bool SetActiveScene(STRCODE sceneId);

	// ------------------------- Entity-related member functions -------------------------

	/**
	 * @brief Create a new entity in the active scene.
	 *
	 * @return Pointer to the newly created entity.
	 */
	Entity* CreateEntity();
	/**
	 * @brief Create a dangling entity. It is not part of any scene.
	 * Useful in object pooling of entities.
	 *
	 * @return Pointer to the newly created entity.
	 */
	Entity* CreateDanglingEntity(bool);

	/**
	 * @brief Find an entity in the active scene.
	 *
	 * @param entityGUID GUID of the entity
	 * @return Pointer to the found entity. If not found, returns nullptr.
	 */
	Entity* FindEntity(std::string& entityGUID);
	/**
	 * @brief Find an entity in the active scene.
	 *
	 * @param entityId UID of the entity
	 * @return Pointer to the found entity. If not found, returns nullptr.
	 */
	Entity* FindEntity(STRCODE entityId);
	/**
	 * @brief Find entities in the active scene which contain a component class.
	 *
	 * @param componentClassName Name of a class which inherits from Component class
	 * @return List of entity pointers.
	 */
	std::list<Entity*> FindEntityWithComponent(const std::string& componentClassName);

	/**
	 * @brief Remove an entity from the active scene.
	 *
	 * @param entityGUID GUID of the entity
	 * @return Boolean representing if entity got removed successfully.
	 */
	bool RemoveEntity(std::string& entityGUID);
	/**
	 * @brief Remove an entity from the active scene.
	 *
	 * @param entityId UID of the entity
	 * @return Boolean representing if entity got removed successfully.
	 */
	bool RemoveEntity(STRCODE entityId);
};

#endif // !_SCENE_MANAGER_H_
