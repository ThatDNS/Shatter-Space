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

	const std::string DATA_FILE = "Assets/SceneManager.json";

	Scene* activeScene = nullptr;

	// UID of the Scene to be set as active (happens in pre-update)
	// Useful when user changes the current scene
	STRCODE toBeSetAsActive = 0;
	Scene* newActiveScene = nullptr;

	// Keep track of file location for each Scene available (this data comes from DATA_FILE json)
	std::unordered_map<STRCODE, std::string> stringUIDToFile;

	// All entity pools used
	// There are different entity pools because different entities have different types of components
	// attached to them. Having different pools ensures both less memory fragmentation as well as cache coherence.
	std::unordered_map<STRCODE, EntityPool*> entityPools;

	// Keep track of all the created scenes as only Scene Manager can destroy & delete scenes
	std::vector<Scene*> allScenes;

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
	 * @return Pointer to the active scene.
	 */
	void SetActiveScene(Scene*);

	///**
	// * @brief Setter function to set the active scene using Scene GUID.
	// *
	// * @param sceneGUID GUID of the scene to be set as active.
	// * @return Bool representing if operation was successful.
	// */
	//bool SetActiveScene(const std::string& sceneGUID);
	///**
	// * @brief Setter function to set the active scene using Scene UID.
	// *
	// * @param sceneId UID of the scene to be set as active.
	// * @return Bool representing if operation was successful.
	// */
	//bool SetActiveScene(STRCODE sceneId);

	/**
	 * @brief Gets the required entity from the appropriate entity pool.
	 * In case no entity pool exists for the passed components, it creates a new entity pool.
	 *
	 * @param components Vector containing components that are part of this entity.
	 * Useful in determining appropriate entity pool.
	 * @return Pointer to the entity.
	 */
	Entity* GetNewEntity(std::vector<std::string>& components);
};

#endif // !_SCENE_MANAGER_H_
