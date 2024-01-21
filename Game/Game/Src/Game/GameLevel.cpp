#include "stdafx.h"
#include "App/app.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"

#include "Game/BallSpawner.h"

void LoadGameEntities(Scene* scene)
{
	// ---------------------- Level Generator ----------------------
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ LevelGeneratorC });
	entity->SetName("LevelGenerator");

	// ---------------------- Ball Spawner ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ BallSpawnerC, ParticlesC });
	entity->SetName("BallSpawner");

	// Set ball spawner data
	BallSpawner* spawner = static_cast<BallSpawner*>(entity->GetComponent(BallSpawnerC));
	spawner->SetMeshObj("Assets/Objects/sphere.obj");

	// ---------------------- Stars Background ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ ParticlesC, StarsControllerC });
	entity->SetName("Background");

	// ---------------------- UI Manager ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ UIManagerC });
	entity->SetName("UI");
}

/**
 * @brief Function used to load up the 1st scene.
 * 
 * Initially, I was loading up this data from a JSON file.
 * However, I had used an external JSON library for loading it.
 * I didn't have time to write a JSON library and can not use external libraries in UbisoftNEXT.
 * So, this is the alternative I'm using :)
 */
void LoadGameScene()
{
	// Create a new scene
	Scene* scene = SceneManager::Get().CreateNewScene();
	scene->SetName("GameLevel");
	scene->SetLoadSceneFunc(LoadGameEntities);
}
