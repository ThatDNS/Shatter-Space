// @file: BallSpawner.cpp
//
// @brief: Cpp file for ball spawner functionalities.

#include "stdafx.h"
#include "Game/BallSpawner.h"
#include "App/app.h"
#include "Engine/Core/Logger.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Particles.h"
#include "Game/SelfDestruct.h"
#include "Game/UIManager.h"

void BallSpawner::Initialize()
{
	ballCounter = 0;
	spawnerMoveSpeed = 10.0f;

	// Start from origin
	GetEntity()->GetTransform().position = Vector3(0.0f, 0.0f, 0.0f);

	// Load the mesh
	mesh.LoadFromObjectFile(meshObjFile);
	RenderSystem::Get().AttachCamera(GetEntity());

	// Find the UIManager
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(UIManagerC);
	if (match.size() == 0)
		Logger::Get().Log("Ball spawner could not find UI Manager", ERROR_LOG);
	else
		uiManager = static_cast<UIManager*>(match.front()->GetComponent(UIManagerC));

	// Find the particles component
	particles = static_cast<Particles*>(entity->GetComponent(ParticlesC));
	particles->SetParticleType(SPEEDLINE);
	particles->SetPositionOffset(Vector3{ 0.0f, 0.0f, 30.0f });
	particles->SetParticleColors(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.5f, 0.5f, 0.5f));
}

void BallSpawner::Update(float deltaTime)
{
	if (!uiManager->IsGamePaused() && !uiManager->IsGameOver())
	{
		// Spawn ball on left click
		if (App::IsKeyPressed(VK_LBUTTON) && !isClickPressed)
		{
			SpawnBall();
			uiManager->DecreaseBalls(1);
			isClickPressed = true;
		}
		else if (!App::IsKeyPressed(VK_LBUTTON))
		{
			isClickPressed = false;
		}

		// Move the ball spawner. Camera moves with it
		GetEntity()->GetTransform().Translate(Vector3(0.0f, 0.0f, spawnerMoveSpeed * (deltaTime / 1000.0f)));
		spawnerMoveSpeed = std::min(spawnerMoveSpeed + (deltaTime / 5000.0f), MAX_SPEED);
		SPEEDLINE_DELAY = 1.0f / spawnerMoveSpeed;

		if (spawnerMoveSpeed > 15.0f)
		{
			// Speedlines
			if (speedLineTimer == 0.0f)
			{
				particles->Emit(1);
				speedLineTimer = SPEEDLINE_DELAY;
			}
			speedLineTimer = std::max(0.0f, speedLineTimer - (deltaTime / 1000.0f));
		}
	}
}

void BallSpawner::SpawnBall()
{
	Scene* scene = SceneManager::Get().GetActiveScene();
	Entity* entity = scene->CreateEntity(ballComponents);
	entity->SetName("Ball_" + std::to_string(ballCounter));

	entity->GetTransform().position = GetEntity()->GetTransform().position;
	// Spawn balls a little in front
	entity->GetTransform().position.z += 5.0f;

	// Load the mesh
	MeshRenderer* mr = static_cast<MeshRenderer*>(entity->GetComponent(MeshRendererC));
	mr->LoadMesh(mesh);
	mr->SetRenderBackSide(false);
	mr->SetMeshColor(Vector3(0.0f, 1.0f, 0.0f));

	// Apply velocity
	RigidBody* rb = static_cast<RigidBody*>(entity->GetComponent(RigidBodyC));
	Vector3 ballDir;
	App::GetMousePos(ballDir.x, ballDir.y);
	// Convert screen coords to the coords used by the engine
	ballDir.x = ((ballDir.x - 0.5f * (float)APP_INIT_WINDOW_WIDTH) / (float)APP_INIT_WINDOW_WIDTH) * 10.0f;
	ballDir.y = ((ballDir.y - 0.5f * (float)APP_INIT_WINDOW_HEIGHT) / (float)APP_INIT_WINDOW_HEIGHT) * 6.0f;
	ballDir.z = 5.0f;
	ballDir.Normalize();
	rb->SetVelocity(ballDir * ballSpeed);

	// Self destuct data
	SelfDestruct* sd = static_cast<SelfDestruct*>(entity->GetComponent(SelfDestructC));
	sd->SetBound(Vector3(0.0f, -20.0f, 0.0f));  // Currently it checks only Y

	// Initialize the ball entity
	entity->Initialize();

	++ballCounter;
	Logger::Get().Log("Balls spawned: " + std::to_string(ballCounter));
}
