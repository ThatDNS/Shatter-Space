// @file: SceneManager.h
//
// @brief: Header file for SceneManager, a singleton responsible for handling all scenes, entities, and components.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-28

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
private:
	DECLARE_SINGLETON(SceneManager)

	const std::string DATA_FILE = "../Assets/SceneManager.json";
	const std::string SAVE_FILE = "../Assets/SavedGame.json";

	std::string mainMenuGUID = "";  // First scene GUID gets stored here
	STRCODE activeSceneId = 0;
	Scene* activeScene = nullptr;

	// Scene to be set as active (happens in pre-update)
	Scene* toBeSetAsActive = nullptr;

	std::list<Scene*> scenesToBeLoaded;
	std::list<Scene*> loadedScenes;
	std::list<Scene*> scenesToBeUnloaded;

	// Keep track of file location for each Scene available
	// (i.e. a scene which either has a JSON or got created by user during runtime)
	std::map <STRCODE, std::string> stringUIDToFile;

	Scene* _recentMostScene = nullptr;

protected:
	void Load();
	void Initialize();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void Destroy();

	void BackToMainMenu();

	// Persistence
	void SaveScene();
	void LoadSavedGame();

public:
	// ------------------------- Scene-related member functions -------------------------
	Scene* GetActiveScene();
	STRCODE GetActiveSceneId();
	bool SetActiveScene(std::string& sceneGUID);
	bool SetActiveScene(STRCODE sceneId);

	Scene* CreateScene();
	bool InitializeRecentlyCreatedScene(Scene*);

	Scene* LoadScene(std::string& sceneGUID);
	Scene* LoadScene(STRCODE sceneId);
	Scene* LoadScene(json::JSON&);
	Scene* FindScene(std::string& sceneGUID);
	Scene* FindScene(STRCODE sceneId);
	bool UnloadScene(std::string& sceneGUID);
	bool UnloadScene(STRCODE sceneId);

	// ------------------------- Entity-related member functions -------------------------
	Entity* CreateEntity();
	Entity* CreateDanglingEntity(bool);

	// Entity look-up always happens in the active scene
	Entity* FindEntity(std::string& entityGUID);
	Entity* FindEntity(STRCODE entityId);
	// Entities in a scene can have same name
	std::list<Entity*> FindEntityByName(std::string& entityName);
	std::list<Entity*> FindEntityWithComponent(std::string& componentClassName);

	bool RemoveEntity(std::string& entityGUID);
	bool RemoveEntity(STRCODE entityId);

	bool sceneChanged = false;
	int persistentData = 0;  // data pesistent across scenes

	friend class Engine;
};

#endif // !_SCENE_MANAGER_H_
