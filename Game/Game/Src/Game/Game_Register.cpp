#include "stdafx.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Components/MeshRenderer.h"

#include "Game/Player.h"

// Registering type classes is important or else we won't be able to use TypeClass.
void Game_Register()
{
	REGISTER_TYPECLASS(Player)
}

/**
 * @brief Function used to load up the 1st scene.
 * 
 * Initially, I was loading up this data from a JSON file.
 * However, I had used an external JSON library for loading it.
 * I didn't have time to write a JSON library and can not use external libraries in UbisoftNEXT.
 * So, this is the alternative I'm using :)
 */
void SetupLevel1()
{
	// Create a new scene
	Scene* scene = SceneManager::Get().CreateNewScene();
	scene->SetName("Level 1");

	// Create a spaceship entity with archetype (mesh renderer, player)
	std::vector<std::string> spaceshipArchetype{ "MeshRenderer", "Player" };
	Entity* entity = scene->CreateEntity(spaceshipArchetype);
	entity->SetName("PlayerSpaceship");

	// Load the data to the components of spaceship entity
	entity->GetTransform().position = Vector3(0.0f, 0.0f, 10.0f);

	Component* component = entity->GetComponent("MeshRenderer");
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(false);

	component = entity->GetComponent("Player");
	Player* player = static_cast<Player*>(component);
	player->SetMoveSpeed(1.5f);

}
