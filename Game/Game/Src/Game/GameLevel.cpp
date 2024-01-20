#include "stdafx.h"
#include "App/app.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/PhysicsSystem.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Canvas.h"
#include "Engine/Components/Particles.h"

#include "Game/Player.h"
#include "Game/BallSpawner.h"


void CreatePlayer(Scene* scene, Vector3& position, Vector3& scale)
{
	// Create a player entity with archetype (mesh renderer, player)
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, RigidBodyC, ParticlesC, PlayerC });
	entity->SetName("Player");

	// Load the data to the components of player entity
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;

	// Load player mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/sphere.obj");
	mr->SetRenderBackSide(true);
	mr->SetMeshColor(Vector3(0.0f, 1.0f, 0.0f));

	RigidBody* rb = static_cast<RigidBody*>(entity->GetComponent(RigidBodyC));
	rb->applyGravity = false;

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
	player->SetMoveSpeed(15.0f);
}

void CreateBallSpawner(Scene* scene)
{
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ BallSpawnerC });
	entity->SetName("Ball_Spawner");

	// Set ball spawner data
	Component* component = entity->GetComponent(BallSpawnerC);
	BallSpawner* spawner = static_cast<BallSpawner*>(component);
	spawner->SetMeshObj("Assets/Objects/sphere.obj");
}

void CreateWall(Scene* scene, Vector3& position, Vector3& scale)
{
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC });
	entity->SetName("Wall");

	// Load the transform data
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;

	// Load wall mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(false);
	mr->SetMeshColor(Vector3(0.0f, 0.0f, 1.0f));
}

void CreateBreakable(Scene* scene, Vector3& position, Vector3& scale)
{
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, RigidBodyC, ParticlesC, BreakableC });
	entity->SetName("Breakable");
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;
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

	// Outer space scene
	PhysicsSystem::Get().SetGravity(-9.8f);

	// ---------------------- Level Generator ----------------------
	/*Vector3 wallScale{ 6.0f, 8.0f, 5.0f };
	Vector3 breakableScale{ 2.0f, 2.0f, 2.0f };*/
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ LevelGeneratorC });
	entity->SetName("Level_Generator");

	// ---------------------- Player Entity ----------------------
	//CreatePlayer(scene, Vector3(00.0f, 6.0f, 12.0f), Vector3(1.0f, 1.0f, 1.0f));
	CreateBallSpawner(scene);

	// ---------------------- Canvas Entity ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ UIManagerC });
	entity->SetName("UI");

	// Add canvas data
	//Component* component = entity->GetComponent(CanvasC);
	//Canvas* canvas = static_cast<Canvas*>(component);
	//canvas->AddText(CanvasText(20, APP_VIRTUAL_HEIGHT - 30, "Level 1"));

}
