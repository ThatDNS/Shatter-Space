// @file: SceneManager.cpp
//
// @brief: Cpp file for SceneManager, a singleton responsible for handling all scenes, entities, and components.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-29

#include "stdafx.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Core/Logger.h"
#include "Engine/Systems/Scene.h"

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
	
	// Get the active scene's data
	THROW_RUNTIME_ERROR(!sceneManagerJSON.hasKey("ActiveSceneGUID"), "Scene Manager must have an active scene.");

	STRCODE activeSceneUID = GetHashCode(sceneManagerJSON["ActiveSceneGUID"].ToString().c_str());
	std::string sceneFileLocation = stringUIDToFile[activeSceneUID];

	std::ifstream sceneInputStream(sceneFileLocation);
	std::string sceneStr((std::istreambuf_iterator<char>(sceneInputStream)), std::istreambuf_iterator<char>());
	json::JSON activeSceneJSON = json::JSON::Load(sceneStr);

	// Create a new scene & load active scene's data in it
	Scene* scene = new Scene();
	scene->Load(activeSceneJSON);
	activeScene = scene;

	Logger::Get().Log("Loaded active scene");
}

void SceneManager::Initialize()
{
	activeScene->Initialize();
}

void SceneManager::PreUpdate()
{
	// If active scene is supposed to be changed, do it
	if (toBeSetAsActive)
	{
		// Create the new scene & set it as active
		std::string sceneFileLocation = stringUIDToFile[toBeSetAsActive];

		std::ifstream sceneInputStream(sceneFileLocation);
		std::string sceneStr((std::istreambuf_iterator<char>(sceneInputStream)), std::istreambuf_iterator<char>());
		json::JSON activeSceneJSON = json::JSON::Load(sceneStr);

		Scene* scene = new Scene();
		scene->Load(activeSceneJSON);
		activeScene = scene;

		// Initialize the newly active scene (required before it can be used)
		activeScene->Initialize();

		toBeSetAsActive = 0;
	}
	activeScene->PreUpdate();
}

void SceneManager::Update()
{
	activeScene->Update();
}

void SceneManager::PostUpdate()
{
	activeScene->PostUpdate();

	// If active scene is supposed to be changed, destory the current active scene
	if (toBeSetAsActive)
	{
		activeScene->Destroy();
		delete activeScene;
		activeScene = nullptr;

		// Do not reset toBeSetAsActive, we haven't created new scene yet (happens in PreUpdate)
	}
}

void SceneManager::Destroy()
{
	activeScene->Destroy();
	delete activeScene;
	activeScene = nullptr;
}

// ------------------------- Scene-related member functions -------------------------

Scene* SceneManager::GetActiveScene()
{
	return activeScene;
}

STRCODE SceneManager::GetActiveSceneId()
{
	if (activeScene != nullptr)
		return activeScene->uid;
	return 0;
}

bool SceneManager::SetActiveScene(const std::string& sceneGUID)
{
	return SetActiveScene(GetHashCode(sceneGUID.c_str()));
}

bool SceneManager::SetActiveScene(STRCODE sceneId)
{
	// Check if sceneId is valid
	if (stringUIDToFile.find(sceneId) == stringUIDToFile.end())
	{
		Logger::Get().Log("Could not change scene due to invalid Scene UID", ERROR_LOG);
		return false;
	}

	toBeSetAsActive = sceneId;
	return true;
}
