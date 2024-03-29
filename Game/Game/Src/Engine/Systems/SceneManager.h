// @file: SceneManager.h
//
// @brief: Header file for SceneManager, a singleton responsible for handling all scenes, entities, and components.

#pragma once
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

class Scene;
class Entity;
class EntityPool;

/**
 * @class SceneManager
 *
 * Scene Manager class provides an API to manage scenes and their entities.
 * 
 * It can keep track of multiple scenes but there can be only 1 active scene at a time.
 * In future, this class can be easily extended to support rendering of multiple scenes at a time.
 */
class SceneManager
{
	friend class Engine;

private:
	DECLARE_SINGLETON(SceneManager)

	Scene* activeScene = nullptr;

	// UID of the Scene to be set as active (happens in pre-update)
	// Useful when user changes the current scene
	STRCODE toBeSetAsActive = 0;
	Scene* newActiveScene = nullptr;

	// All entity pools used
	// There are different entity pools because different entities have different types of components
	// attached to them. Having different pools ensures both less memory fragmentation as well as cache coherence.
	std::unordered_map<STRCODE, EntityPool*> entityPools;

	// Keep track of all the created scenes as only Scene Manager can destroy & delete scenes
	// NOTE: Developers are allowed to create new scenes using SceneManager & change the active scene, but they can't delete scenes.
	std::vector<Scene*> allScenes;

	// Persistent data between scenes
	std::unordered_map<STRCODE, std::string> persistentData;

protected:
	/**
	 * @brief Load the active scene data.
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
	void Update(float);

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
	/**
	 * @brief Create a new empty scene
	 *
	 * @return Pointer to the created scene.
	 */
	Scene* CreateNewScene();

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
	 * @brief Set the passed scene as the active scene.
	 *
	 * @param scene Pointer to the active scene.
	 */
	void SetActiveScene(Scene* scene);

	/**
	 * @brief Gets the required entity from the appropriate entity pool.
	 * In case no entity pool exists for the passed components, it creates a new entity pool.
	 *
	 * @param components Vector containing components that are part of this entity.
	 * Useful in determining appropriate entity pool.
	 * @return Pointer to the entity.
	 */
	Entity* GetNewEntity(std::vector<ComponentType>& components);

	/**
	 * @brief Store data that remains persistent as long as the application runs.
	 *
	 * @param hashkey Hash key to store the data.
	 * @param dataStr String representing actual data.
	 */
	void StorePersistentData(STRCODE hashkey, std::string& dataStr);

	/**
	 * @brief Check if the data is available for given hashkey.
	 *
	 * @param hashkey Hash key to store the data.
	 * @return Is the data available?
	 */
	bool HasPersistentData(STRCODE hashkey);

	/**
	 * @brief Get the persistent data.
	 *
	 * @param hashkey Hash key to store the data.
	 * @return String representing actual data.
	 */
	std::string GetPersistentData(STRCODE hashkey);
};

#endif // !_SCENE_MANAGER_H_
