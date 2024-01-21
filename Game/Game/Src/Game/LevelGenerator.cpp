// @file: LevelGenerator.cpp
//
// @brief: Cpp file for LevelGenerator class responsible for generating level as per the position of player.

#include "stdafx.h"
#include "App/app.h"
#include "Game/LevelGenerator.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Particles.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Random.h"
#include "Game/SelfDestruct.h"
#include "Game/Breakable.h"
#include "Game/BallSpawner.h"

void LevelGenerator::Initialize()
{
	_pyramidCounter = 0;
	maxSightDistance = 150.0f;
	lastSpawnDistance = 0.0f;

	// Find the player entity and cache it
	std::list<Entity*> matchedEntities = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(BallSpawnerC);
	if (matchedEntities.size() == 0)
	{
		Logger::Get().Log("Could not find ball spawner");
	}
	else
	{
		ballSpawnerEntity = matchedEntities.front();
		ballSpawner = static_cast<BallSpawner*>(ballSpawnerEntity->GetComponent(BallSpawnerC));
	}
}

void LevelGenerator::Update(float deltaTime)
{
	float playerPosition = ballSpawnerEntity->GetTransform().position.z;
	float canSeeTill = playerPosition + maxSightDistance;
	while (lastSpawnDistance + SEPARATION_DIST <= canSeeTill)
	{
		// Spawn level
		lastSpawnDistance += SEPARATION_DIST;
		SpawnLevel(lastSpawnDistance);
	}
}

void LevelGenerator::CreateWallEntity(Vector3& position, Vector3& scale)
{
	Entity* entity = SceneManager::Get().GetActiveScene()->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, SelfDestructC });
	entity->SetName("Wall");

	// Load the transform data
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;

	// Load wall mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(false);
	mr->SetMeshColor(Vector3(0.8f, 0.8f, 0.8f));
	
	// Initialize
	entity->Initialize();
}

void LevelGenerator::CreateBreakableEntity(Vector3& position, Vector3& scale, Vector3& rotation, BreakableType breakableType)
{
	Entity* entity = SceneManager::Get().GetActiveScene()->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, RigidBodyC, ParticlesC, BreakableC, SelfDestructC });
	entity->SetName("Breakable");
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;
	entity->GetTransform().rotation = rotation;

	// Set breakable type
	Breakable* breakable = static_cast<Breakable*>(entity->GetComponent(BreakableC));
	breakable->SetBreakableType(breakableType);

	// Initialize
	entity->Initialize();
}

void LevelGenerator::SpawnLevel(float zPos)
{
	if (_pyramidCounter < 3)
	{
		// Spawn breakable pyramid
		Vector3 wallScale{ 6.0f, 8.0f, 5.0f };
		Vector3 breakableScale{ 2.0f, 2.0f, 2.0f };
		Vector3 breakableRotation{ 0.0f, 0.0f, 0.0f };

		CreateWallEntity(Vector3(-15.0f, -15.0f, zPos), wallScale);
		CreateWallEntity(Vector3(0.0f, -15.0f, zPos), wallScale);
		CreateWallEntity(Vector3(15.0f, -15.0f, zPos), wallScale);
		CreateBreakableEntity(Vector3(-15.0f, -2.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		CreateBreakableEntity(Vector3(0.0f, -2.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		CreateBreakableEntity(Vector3(15.0f, -2.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		++_pyramidCounter;
	}
	else
	{
		// Spawn breakable plane
		Vector3 breakableScale{ 5.0f, 5.0f, 5.0f };
		Vector3 breakableRotation{ 0.0f, (float)PI / 2.0f, 0.0f };
		CreateBreakableEntity(Vector3(0.0f, -6.0f, zPos), breakableScale, breakableRotation, BreakableType::Plane);
		_pyramidCounter = 0;
	}

	// Randomly generate stars
	if (Random::Get().Float() < STAR_PROBABILITY)
	{
		// X: [-35, -25] or [25, 35]
		// Y: [30, 40]
		float xPos = Random::Get().Float() * 10.0f + 25.0f;
		if (Random::Get().Float() < 0.5f) xPos = -xPos;

		// Adjust spawn position as per player / spawner speed
		// [10, 50] -> [-3/2 * separation, -1/2 * separation]
		float speed = ballSpawner->GetSpawnerSpeed();
		float zPosition = zPos - (3.0f * SEPARATION_DIST / 2.0f) + (SEPARATION_DIST * (speed - 10.0f) / 40.0f);

		Vector3 position{ xPos, Random::Get().Float() * 10.0f + 30.0f, zPosition };
		Vector3 scale{ 2.0f, 2.0f, 2.0f };
		Vector3 rotation{ (float)PI / 2.0f, 0.0f, 0.0f };
		CreateBreakableEntity(position, scale, rotation, BreakableType::Star);
	}
}

