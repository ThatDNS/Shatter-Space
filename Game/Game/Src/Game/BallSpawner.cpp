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
#include "Game/Ball.h"

void BallSpawner::Initialize()
{
	// Load the mesh
	mesh.LoadFromObjectFile(meshObjFile);
	RenderSystem::Get().AttachCamera(GetEntity());
}

void BallSpawner::Update(float deltaTime)
{
	// Spawn ball on left click
	if (App::IsKeyPressed(VK_LBUTTON) && !isClickPressed)
	{
		SpawnBall();
		isClickPressed = true;
	}
	else if (!App::IsKeyPressed(VK_LBUTTON))
	{
		isClickPressed = false;
	}

	// Move the ball spawner. Camera moves with it
	GetEntity()->GetTransform().Translate(Vector3(0.0f, 0.0f, spawnerMoveSpeed * (deltaTime / 1000.0f)));
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

	// Ball script data
	Ball* ball = static_cast<Ball*>(entity->GetComponent(BallC));
	ball->minYPos = -20.0f;

	// Initialize the ball entity
	entity->Initialize();

	++ballCounter;
	Logger::Get().Log("Balls spawned: " + std::to_string(ballCounter));
}
