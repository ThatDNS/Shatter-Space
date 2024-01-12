// @file: SceneManager.cpp
//
// @brief: Cpp file for SceneManager, a singleton responsible for handling all scenes, entities, and components.

#include "stdafx.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Core/Logger.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"
#include "Engine/Pools/EntityPool.h"

void SceneManager::Initialize()
{
	activeScene->Initialize();
}

void SceneManager::PreUpdate()
{
	// If active scene is supposed to be changed, do it here
	if (newActiveScene != nullptr)
	{
		activeScene = newActiveScene;
		newActiveScene = nullptr;
	}

	activeScene->PreUpdate();
}

void SceneManager::Update(float deltaTime)
{
	activeScene->Update(deltaTime);
}

void SceneManager::PostUpdate()
{
	activeScene->PostUpdate();
}

void SceneManager::Destroy()
{
	// Active scene is also a part of allScenes
	for (Scene* scene : allScenes)
	{
		scene->Destroy();
		delete scene;
	}
}

Scene* SceneManager::CreateNewScene()
{
	Scene* scene = new Scene();
	allScenes.push_back(scene);
	Logger::Get().Log("Created new scene");
	
	// If a new scene is being created and there is no active scene then it is probably happening
	// at start of the game. A safe-side action here is to directly set this scene as active scene
	// instead of doing it via SetActiveScene, which causes active scene to get updated in PreUpdate.
	if (activeScene == nullptr)
		activeScene = scene;
	
	return scene;
}

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

void SceneManager::SetActiveScene(Scene* scene)
{
	newActiveScene = scene;
}

Entity* SceneManager::GetNewEntity(std::vector<std::string>& components)
{
	// Create the string whose hash is used as key for the Entity pool
	std::string compStr = "";
	for (std::string& component : components)
		compStr += component;

	EntityPool* relevantPool = nullptr;
	STRCODE compHash = GetHashCode(compStr.c_str());
	if (entityPools.find(compHash) == entityPools.end())
	{
		// Create new entity pool
		relevantPool = new EntityPool(components);
		// Track it
		entityPools[compHash] = relevantPool;
	}
	else
	{
		relevantPool = entityPools[compHash];
	}
	return static_cast<Entity*>(relevantPool->GetFreeObject());
}
