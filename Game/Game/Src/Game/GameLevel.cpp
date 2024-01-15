#include "stdafx.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/Particles.h"

#include "Game/Player.h"


void CreatePlayer(Scene* scene, Vector3& position, Vector3& scale)
{
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, RigidBodyC, ParticlesC, PlayerC });
	entity->SetName("Player");

	// Load the data to the components of player entity
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;

	// Load player mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cone.obj");
	mr->SetRenderBackSide(false);

	// Set box collider data
	component = entity->GetComponent(BoxColliderC);
	BoxCollider* boxCollider = static_cast<BoxCollider*>(component);
	boxCollider->SetShouldRender(true);

	// Particles settings
	component = entity->GetComponent(ParticlesC);
	Particles* particles = static_cast<Particles*>(component);
	particles->SetParticleType(PROPULSION);

	// Set player script data
	component = entity->GetComponent(PlayerC);
	Player* player = static_cast<Player*>(component);
	player->SetMoveSpeed(1.5f);
}

void CreateWall(Scene* scene, Vector3& position, Vector3& scale)
{
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC });
	entity->SetName("Wall1");

	// Load the transform data
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;

	// Load wall mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(true);
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

	// ---------------------- Walls Entities ----------------------
	CreateWall(scene, Vector3(-9.0f, 0.0f, 20.0f), Vector3(0.2f, 8.0f, 5.0f));
	CreateWall(scene, Vector3(-3.0f, -1.0f, 20.0f), Vector3(0.2f, 6.0f, 5.0f));
	CreateWall(scene, Vector3(3.0f, 1.0f, 20.0f), Vector3(0.2f, 6.0f, 5.0f));
	CreateWall(scene, Vector3(9.0f, 0.0f, 20.0f), Vector3(0.2f, 8.0f, 5.0f));

	// ---------------------- Player Entity ----------------------
	// Create a player entity with archetype (mesh renderer, player)
	CreatePlayer(scene, Vector3(0.0f, 0.0f, 15.0f), Vector3(1.0f, 1.0f, 1.0f));

}
