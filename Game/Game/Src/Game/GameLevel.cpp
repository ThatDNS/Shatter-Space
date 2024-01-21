#include "stdafx.h"
#include "App/app.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/PhysicsSystem.h"
#include "Engine/Systems/RenderSystem.h"
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

void LoadGameEntities(Scene* scene)
{
	// ---------------------- Level Generator ----------------------
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ LevelGeneratorC });
	entity->SetName("LevelGenerator");

	// ---------------------- Ball Spawner ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ BallSpawnerC, ParticlesC });
	entity->SetName("BallSpawner");

	// ---------------------- Stars Background ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ ParticlesC, StarsControllerC });
	entity->SetName("Background");

	// ---------------------- UI Manager ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ UIManagerC });
	entity->SetName("UI");

	// ---------------------- Aim Assist ----------------------
	entity = scene->CreateEntity(std::vector<ComponentType>{ ParticlesC, AimAssistC });
	entity->SetName("AimAssist");

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
