// @file: SceneManager.cpp
//
// @brief: Cpp file for SceneManager, a singleton responsible for handling all scenes, entities, and components.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-28

#include "stdafx.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

/**
 * @brief Load Scene Manager saved data from SceneManager.json file
 */
void SceneManager::Load()
{
	std::ifstream inputStream(DATA_FILE);
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON sceneManagerJSON = json::JSON::Load(str);

	// Store mapping of scene id to scene path location
	// SceneManager must know about every scene in existance
	THROW_RUNTIME_ERROR(!sceneManagerJSON.hasKey("AllScenes"), "Scene Manager must have GUID & path data of all available scenes.");
	json::JSON allScenes = sceneManagerJSON["AllScenes"];
	for (json::JSON& sceneInfo : allScenes.ArrayRange())
	{
		if (sceneInfo.hasKey("GUID") && sceneInfo.hasKey("Path"))
		{
			std::string sceneGUID = sceneInfo["GUID"].ToString();
			std::string scenePath = sceneInfo["Path"].ToString();

			STRCODE sceneId = GetHashCode(sceneGUID.c_str());
			stringUIDToFile[sceneId] = scenePath;
		}
	}
	
	// Get the active scene GUID. Convert it to STRCODE
	THROW_RUNTIME_ERROR(!sceneManagerJSON.hasKey("ActiveSceneGUID"), "Scene Manager must have an active scene.");
	activeSceneId = GetHashCode(sceneManagerJSON["ActiveSceneGUID"].ToString().c_str());

	// Load the scenes to be loaded at the beginning of the game
	if (sceneManagerJSON.hasKey("LoadedScenes"))
	{
		json::JSON scenesJSON = sceneManagerJSON["LoadedScenes"];
		for (json::JSON& sceneGUIDJSON : scenesJSON.ArrayRange())
		{
			THROW_RUNTIME_ERROR(!sceneGUIDJSON.hasKey("GUID"), "A loaded scene must have a GUID.");

			if (mainMenuGUID == "")
				mainMenuGUID = sceneGUIDJSON["GUID"].ToString();

			STRCODE sceneID = GetHashCode(sceneGUIDJSON["GUID"].ToString().c_str());
			if (stringUIDToFile.find(sceneID) != stringUIDToFile.end())
			{
				// Load up Scene JSON file & send the JSON object to Scene
				std::string sceneFileLocation = stringUIDToFile[sceneID];

				std::ifstream sceneInputStream(sceneFileLocation);
				std::string sceneStr((std::istreambuf_iterator<char>(sceneInputStream)), std::istreambuf_iterator<char>());
				json::JSON sceneJSON = json::JSON::Load(sceneStr);

				// Load the scene & track it
				Scene* scene = LoadScene(sceneJSON);

				// If this is the active scene, set it
				if (sceneID == activeSceneId)
				{
					activeScene = scene;
					// active scene has to be enabled, or else it won't update and render
					activeScene->isEnabled = true;
				}
			}
		}
	}
	LOG("Loaded SceneManager: " << scenesToBeLoaded.size() << " scenes loaded from JSON.");
}

/**
 * @brief Initialize all the scenes.
 */
void SceneManager::Initialize()
{
	// Initialize happens before first PreUpdate. So it must be called on scenesToBeLoaded
	for (Scene* scene : scenesToBeLoaded)
	{
		scene->Initialize();
	}
}

/**
 * @brief Load the to-be-loaded scenes.
 */
void SceneManager::PreUpdate()
{
	sceneChanged = false;

	for (Scene* scene : scenesToBeLoaded)
	{
		loadedScenes.push_back(scene);
	}
	scenesToBeLoaded.clear();
	// Check if active scene got modified
	if (toBeSetAsActive != nullptr)
	{
		activeScene = toBeSetAsActive;
		// Active scene must be enabled
		activeScene->isEnabled = true;
		toBeSetAsActive = nullptr;
	}
	// Call pre update on all loaded scenes
	// Useful for adding any entities scheduled to be added
	for (Scene* scene : loadedScenes)
	{
		if (scene->isEnabled)
		{
			scene->PreUpdate();
		}
	}
}

/**
 * @brief Update all the enabled scenes.
 */
void SceneManager::Update()
{
	for (Scene* scene : loadedScenes)
	{
		if (scene->isEnabled && !sceneChanged)
		{
			scene->Update();
		}
	}
}

/**
 * @brief Unload the to-be-unloaded scenes.
 */
void SceneManager::PostUpdate()
{
	// Call post update on all loaded scenes
	// Useful for deleting any entities scheduled to be deleted
	for (Scene* scene : loadedScenes)
	{
		if (scene->isEnabled)
		{
			scene->PostUpdate();
		}
	}

	for (Scene* scene : scenesToBeUnloaded)
	{
		scene->Destroy();
		delete scene;
		loadedScenes.remove(scene);
	}
	scenesToBeUnloaded.clear();
}

/**
 * @brief Save the JSON of the scene
 */
void SceneManager::SaveScene()
{
	// Build the save game json (actually a scene)
	json::JSON savedJSON;
	for (Scene* scene : loadedScenes)
	{
		if (scene->isEnabled && !sceneChanged)
		{
			json::JSON sceneJSON = scene->Save();
			savedJSON["Scene"] = sceneJSON;

			// Saving is supported with just one scene :)
			break;
		}
	}
	std::ofstream ostrm(SAVE_FILE);
	ostrm << savedJSON.dump() << std::endl;
}

void SceneManager::LoadSavedGame()
{
	STRCODE currentSceneId = GetActiveSceneId();

	// Load saved scene
	std::ifstream inputStream(SAVE_FILE);
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON savedSceneJSON = json::JSON::Load(str);

	Scene* scene = LoadScene(savedSceneJSON);
	LOG("Loading new scene - " << scene->GetGUID());

	// Set it as active and then initialize the scene
	std::string sceneGUID = scene->GetGUID();
	SetActiveScene(sceneGUID);
	InitializeRecentlyCreatedScene(scene);

	// Unload current scene
	UnloadScene(currentSceneId);
}

void SceneManager::BackToMainMenu()
{
	STRCODE currentSceneId = GetActiveSceneId();

	// Load new scene
	Scene* scene = LoadScene(mainMenuGUID);
	LOG("Loading new scene - " << scene->GetGUID());

	// Set it as active and then initialize the scene
	SetActiveScene(mainMenuGUID);
	InitializeRecentlyCreatedScene(scene);

	// Unload current scene
	UnloadScene(currentSceneId);
}

/**
 * @brief Destory all the scenes.
 */
void SceneManager::Destroy()
{
	if (scenesToBeLoaded.size() != 0)
		loadedScenes.merge(scenesToBeLoaded);
	if (scenesToBeUnloaded.size() != 0)
		loadedScenes.merge(scenesToBeUnloaded);
	
	for (Scene* scene : loadedScenes)
	{
		scene->Destroy();
		delete scene;
	}
	scenesToBeUnloaded.clear();
	loadedScenes.clear();
	scenesToBeLoaded.clear();
}

// ------------------------- Scene-related member functions -------------------------

/**
 * @brief Create a new scene. Scene Manager automatically keeps track of this scene.
 * 
 * @return Pointer to the created scene.
 */
Scene* SceneManager::CreateScene()
{
	// This scene might or might not have a JSON file. Can not add it to stringUIDToFile
	Scene* scene = new Scene();

	// Created scenes automatically get added
	scenesToBeLoaded.push_back(scene);
	_recentMostScene = scene;
	
	return scene;
}

Scene* SceneManager::LoadScene(std::string& sceneGUID)
{
	return LoadScene(GetHashCode(sceneGUID.c_str()));
}

Scene* SceneManager::LoadScene(STRCODE sceneID)
{
	if (stringUIDToFile.find(sceneID) != stringUIDToFile.end())
	{
		// Load up Scene JSON file & send the JSON object to Scene
		std::string sceneFileLocation = stringUIDToFile[sceneID];

		std::ifstream sceneInputStream(sceneFileLocation);
		std::string sceneStr((std::istreambuf_iterator<char>(sceneInputStream)), std::istreambuf_iterator<char>());
		json::JSON sceneJSON = json::JSON::Load(sceneStr);

		// Load the scene & track it
		Scene* scene = LoadScene(sceneJSON);

		return scene;
	}
	return nullptr;
}

/**
 * @brief Load a scene using JSON object. Scene Manager automatically keeps track of this scene.
 * 
 * @param sceneJSON json object containing scene data.
 * @return Pointer to the created scene.
 */
Scene* SceneManager::LoadScene(json::JSON& sceneJSON)
{
	Scene* scene = new Scene();
	// Active scene might get accessed for spawning entities on scene load
	// So if no active scene exists, mark this created scene as active
	if (activeScene == nullptr)
		activeScene = scene;

	scene->Load(sceneJSON);

	// Loaded scenes automatically get added
	scenesToBeLoaded.push_back(scene);
	_recentMostScene = scene;

	return scene;
}

/**
 * @brief Getter function to get the active scene.
 *
 * @return Pointer to the active scene.
 */
Scene* SceneManager::GetActiveScene()
{
	if (toBeSetAsActive != nullptr)
		return toBeSetAsActive;
	return activeScene;
}

/**
 * @brief Getter function to get the active scene.
 *
 * @return ID of the active scene.
 */
STRCODE SceneManager::GetActiveSceneId()
{
	if (toBeSetAsActive != nullptr)
		return toBeSetAsActive->uid;
	return activeScene->uid;
}

/**
 * @brief Setter function to set the active scene using Scene GUID.
 *
 * @param sceneGUID GUID of the scene to be set as active.
 * @return Pointer to the active scene.
 */
bool SceneManager::SetActiveScene(std::string& sceneGUID)
{
	STRCODE sceneId = GetHashCode(sceneGUID.c_str());
	return SetActiveScene(sceneId);
}

/**
 * @brief Set a scene as active scene. This scene must be loaded first.
 *
 * @param sceneId UID of the scene to be set as active.
 * @return Pointer to the active scene.
 */
bool SceneManager::SetActiveScene(STRCODE sceneId)
{
	// Look for the scene in scenesToBeLoaded & loadedScenes
	for (Scene* scene : scenesToBeLoaded)
	{
		if (scene->GetUID() == sceneId)
		{
			// TODO: Deprecate toBeSetAsActive
			//toBeSetAsActive = scene;
			activeScene = scene;
			return true;
		}
	}
	for (Scene* scene : loadedScenes)
	{
		if (scene->GetUID() == sceneId)
		{
			// Ensure that this scene is not scheduled to be deleted
			auto it = std::find_if(scenesToBeUnloaded.begin(), scenesToBeUnloaded.end(),
								   [sceneId](Scene* sc) {
										return sc->GetUID() == sceneId;
									});
			THROW_RUNTIME_ERROR(it != scenesToBeUnloaded.end(), "Error! The scene being set as active does not exist anymore.");
			//toBeSetAsActive = scene;
			activeScene = scene;
			return true;
		}
	}
	return false;
}

bool SceneManager::InitializeRecentlyCreatedScene(Scene* scene)
{
	if (_recentMostScene->GetUID() == scene->GetUID())
	{
		scene->Initialize();
		return true;
	}
	return false;
}

/**
 * @brief Find a scene among the loaded scenes.
 *
 * @param sceneGUID GUID of the scene.
 * @return Pointer to the scene. if not found, returns nullptr.
 */
Scene* SceneManager::FindScene(std::string& sceneGUID)
{
	STRCODE sceneId = GetHashCode(sceneGUID.c_str());
	return FindScene(sceneId);
}

/**
 * @brief Find a scene among the loaded scenes.
 *
 * @param sceneId UID of the scene.
 * @return Pointer to the scene. if not found, returns nullptr.
 */
Scene* SceneManager::FindScene(STRCODE sceneId)
{
	for (Scene* scene : loadedScenes)
	{
		if (scene->GetUID() == sceneId)
		{
			return scene;
		}
	}
	return nullptr;
}

/**
 * @brief Unload a scene.
 *
 * @param sceneGUID GUID of the scene.
 * @return Boolean represeting if unloading was successful.
 */
bool SceneManager::UnloadScene(std::string& sceneGUID)
{
	STRCODE sceneId = GetHashCode(sceneGUID.c_str());
	return UnloadScene(sceneId);
}

/**
 * @brief Unload a scene.
 *
 * @param sceneId UID of the scene.
 * @return Boolean represeting if unloading was successful.
 */
bool SceneManager::UnloadScene(STRCODE sceneId)
{
	// Ensure that user is not trying to remove the active scene
	Scene* actualActiveScene = (toBeSetAsActive == nullptr) ? activeScene : toBeSetAsActive;
	if (sceneId == actualActiveScene->GetUID())
		return false;

	for (Scene* scene : loadedScenes)
	{
		if (scene->GetUID() == sceneId)
		{
			scenesToBeUnloaded.push_back(scene);
			return true;
		}
	}
	return false;
}

// ------------------------- Entity-related member functions -------------------------

/**
 * @brief Create a new entity in the active scene.
 *
 * @return Pointer to the newly created entity.
 */
Entity* SceneManager::CreateEntity()
{
	return activeScene->CreateEntity();
}

/**
 * @brief Create a dangling entity. It is not part of any scene.
 *
 * @return Pointer to the newly created entity.
 */
Entity* SceneManager::CreateDanglingEntity(bool forObjectPool)
{
	// We use active scene to create it because SceneManager doesn't have the permission
	// to create an entity. All entities must be created via a Scene.
	return activeScene->CreateDanglingEntity(forObjectPool);
}

/**
 * @brief Find an entity in the active scene.
 *
 * @param entityGUID GUID of the entity
 * @return Pointer to the found entity. If not found, returns nullptr.
 */
Entity* SceneManager::FindEntity(std::string& entityGUID)
{
	return activeScene->FindEntity(entityGUID);
}

/**
 * @brief Find an entity in the active scene.
 *
 * @param entityId UID of the entity
 * @return Pointer to the found entity. If not found, returns nullptr.
 */
Entity* SceneManager::FindEntity(STRCODE entityId)
{
	return activeScene->FindEntity(entityId);
}

/**
 * @brief Find entities in the active scene by name.
 *
 * @param entityName Name of the entity
 * @return List of entity pointers.
 */
std::list<Entity*> SceneManager::FindEntityByName(std::string& entityName)
{
	return activeScene->FindEntityByName(entityName);
}

/**
 * @brief Find entities in the active scene which contain a component class.
 *
 * @param componentClassName Name of a class which inherits from Component class
 * @return List of entity pointers.
 */
std::list<Entity*> SceneManager::FindEntityWithComponent(std::string& componentClassName)
{
	return activeScene->FindEntityWithComponent(componentClassName);
}

/**
 * @brief Remove an entity from the active scene.
 *
 * @param entityGUID GUID of the entity
 * @return Boolean representing if entity got removed successfully.
 */
bool SceneManager::RemoveEntity(std::string& entityGUID)
{
	return activeScene->RemoveEntity(entityGUID);
}

/**
 * @brief Remove an entity from the active scene.
 *
 * @param entityId UID of the entity
 * @return Boolean representing if entity got removed successfully.
 */
bool SceneManager::RemoveEntity(STRCODE entityId)
{
	return activeScene->RemoveEntity(entityId);
}