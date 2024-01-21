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
#include "Game/DoorOpener.h"
#include "Game/UIManager.h"

void LevelGenerator::Initialize()
{
	maxSightDistance = 150.0f;
	lastSpawnDistance = 0.0f;
	isFirstDoor = true;
	_countIter = 0;

	// Find the player entity and cache it
	std::list<Entity*> matchedEntities = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(BallSpawnerC);
	if (matchedEntities.size() == 0)
	{
		Logger::Get().Log("Level generator could not find ball spawner", ERROR_LOG);
	}
	else
	{
		ballSpawnerEntity = matchedEntities.front();
		ballSpawner = static_cast<BallSpawner*>(ballSpawnerEntity->GetComponent(BallSpawnerC));
	}

	// Find the UIManager
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(UIManagerC);
	if (match.size() == 0)
		Logger::Get().Log("Level generator could not find UI Manager", ERROR_LOG);
	else
		uiManager = static_cast<UIManager*>(match.front()->GetComponent(UIManagerC));
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

Entity* LevelGenerator::CreateWallEntity(Vector3& position, Vector3& scale, bool isDoor, bool opensLeft)
{
	std::vector<ComponentType> comps{ MeshRendererC, BoxColliderC, SelfDestructC };
	if (isDoor)
		comps.push_back(DoorOpenerC);

	Entity* entity = SceneManager::Get().GetActiveScene()->CreateEntity(comps);
	entity->SetName("Wall");

	// Load the transform data
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;

	// Load wall mesh
	Component* component = entity->GetComponent(MeshRendererC);
	MeshRenderer* mr = static_cast<MeshRenderer*>(component);
	mr->LoadMesh("Assets/Objects/cube.obj");
	mr->SetRenderBackSide(false);
	if (isDoor)
	{
		mr->SetMeshColor(Vector3(1.0f, 0.0f, 0.0f));
		// Attach the breakable entity to the door to know when to open it
		DoorOpener* doorOpener = static_cast<DoorOpener*>(entity->GetComponent(DoorOpenerC));
		doorOpener->SetOpensLeft(opensLeft);
		doorOpener->SetOpenDoor(false);
	}
	else
	{
		mr->SetMeshColor(Vector3(0.8f, 0.8f, 0.8f));
	}

	// Initialize
	entity->Initialize();

	return entity;
}

Entity* LevelGenerator::CreateBreakableEntity(Vector3& position, Vector3& scale, Vector3& rotation, BreakableType breakableType)
{
	Entity* entity = SceneManager::Get().GetActiveScene()->CreateEntity(std::vector<ComponentType>{ MeshRendererC, BoxColliderC, RigidBodyC, ParticlesC, BreakableC, SelfDestructC });
	entity->SetName("Breakable");
	entity->GetTransform().position = position;
	entity->GetTransform().scale = scale;
	entity->GetTransform().rotation = rotation;

	// Set breakable type
	Breakable* breakable = static_cast<Breakable*>(entity->GetComponent(BreakableC));
	breakable->SetBreakableType(breakableType);
	breakable->ClearDoorOpeners();
	if (breakableType == BreakableType::Plane && Random::Get().Float() < PLANE_MOVE_PROBABILITY)
	{
		breakable->SetMoveVertically(true);
	}
	else
	{
		breakable->SetMoveVertically(false);
	}

	// Initialize
	entity->Initialize();

	return entity;
}

void LevelGenerator::SpawnLevel(float zPos)
{
	++_countIter;
	// Generate a door at every 10th step
	if (_countIter % 10 == 0)
	{
		// Closed door
		Vector3 wallScale{ 6.0f, 8.0f, 1.0f };
		Vector3 breakableScale{ 2.0f, 2.0f, 2.0f };
		Vector3 breakableRotation{ 0.0f, 0.0f, 0.0f };
		Entity* breakableE = CreateBreakableEntity(Vector3(0.0f, 4.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		Entity* doorLE = CreateWallEntity(Vector3(-6.0f, -5.0f, zPos), wallScale, true, true);
		Entity* doorRE = CreateWallEntity(Vector3(6.0f, -5.0f, zPos), wallScale, true, false);

		// Attach the doors
		Breakable* breakable = static_cast<Breakable*>(breakableE->GetComponent(BreakableC));
		breakable->AttachDoorOpener(static_cast<DoorOpener*>(doorLE->GetComponent(DoorOpenerC)));
		breakable->AttachDoorOpener(static_cast<DoorOpener*>(doorRE->GetComponent(DoorOpenerC)));

		if (isFirstDoor)
		{
			UIBuffer doorMsg;
			doorMsg.position.x = APP_VIRTUAL_WIDTH / 2 - 135;
			doorMsg.position.y = APP_VIRTUAL_HEIGHT - 70;
			doorMsg.timeRemaining = 5.0f;
			doorMsg.color = Vector3(1.0f, 0.5f, 0.0f);
			doorMsg.project = false;
			doorMsg.text = "Break the ICE on Red door to open it!";
			uiManager->ScheduleRender(doorMsg);
			uiManager->SetTutorialTimer(5.0f);
			isFirstDoor = false;
		}
	}
	// Randomly generate breakable objects
	else if (_countIter > 4 && Random::Get().Float() < PLANE_PROBABILITY)
	{
		// Spawn breakable plane
		Vector3 breakableScale{ 5.0f, 5.0f, 5.0f };
		Vector3 breakableRotation{ 0.0f, (float)PI / 2.0f, 0.0f };
		CreateBreakableEntity(Vector3(0.0f, -6.0f, zPos), breakableScale, breakableRotation, BreakableType::Plane);
	}
	else
	{
		// Spawn breakable pyramid
		Vector3 wallScale{ 6.0f, 8.0f, 5.0f };
		Vector3 breakableScale{ 2.0f, 2.0f, 2.0f };
		Vector3 breakableRotation{ 0.0f, 0.0f, 0.0f };

		CreateWallEntity(Vector3(-15.0f, -16.0f, zPos), wallScale);
		CreateWallEntity(Vector3(15.0f, -16.0f, zPos), wallScale);
		CreateBreakableEntity(Vector3(-15.0f, -3.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		CreateBreakableEntity(Vector3(15.0f, -3.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		
		if (Random::Get().Float() > 0.2f)
		{
			CreateWallEntity(Vector3(0.0f, -16.0f, zPos), wallScale);
			CreateBreakableEntity(Vector3(0.0f, -3.0f, zPos), breakableScale, breakableRotation, BreakableType::Pyramid);
		}
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

