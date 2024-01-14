#include "stdafx.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/BoxCollider.h"

#include "Game/Player.h"

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

	// ---------------------- Player Entity ----------------------
	// Create a player entity with archetype (mesh renderer, player)
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, PlayerC });
	entity->SetName("PlayerGameObject");

	// Load the data to the components of player entity
	entity->GetTransform().position = Vector3(0.0f, 0.0f, 10.0f);

	// Load player mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(false);

	// Set box collider data
	component = entity->GetComponent(BoxColliderC);
	BoxCollider* boxCollider = static_cast<BoxCollider*>(component);
	boxCollider->SetShouldRender(false);

	// Set player script data
	component = entity->GetComponent(PlayerC);
	Player* player = static_cast<Player*>(component);
	player->SetMoveSpeed(1.5f);

	// ---------------------- Wall Entity ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC });
	entity->SetName("Wall");

	// Load the data to the components of spaceship entity
	entity->GetTransform().position = Vector3(5.0f, 0.0f, 10.0f);
	entity->GetTransform().scale = Vector3(1.0f, 4.0f, 5.0f);

	// Load wall mesh
	component = entity->GetComponent(MeshRendererC);
	mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(false);
}
