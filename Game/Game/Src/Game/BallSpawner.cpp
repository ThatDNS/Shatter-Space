// @file: BallSpawner.cpp
//
// @brief: Cpp file for ball spawner functionalities.

#include "stdafx.h"
#include "Game/BallSpawner.h"
#include "App/app.h"
#include "Engine/Core/Logger.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/RigidBody.h"
#include "Game/Ball.h"

void BallSpawner::Initialize()
{
	// Load the mesh
	mesh.LoadFromObjectFile(meshObjFile);
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
}

void BallSpawner::SpawnBall()
{
	Scene* scene = SceneManager::Get().GetActiveScene();
	Entity* entity = scene->CreateEntity(ballComponents);
	entity->SetName("Ball_" + std::to_string(ballCounter));

	entity->GetTransform().position = Vector3(0.0f, 0.0f, 5.0f);

	// Load the mesh
	MeshRenderer* mr = static_cast<MeshRenderer*>(entity->GetComponent(MeshRendererC));
	mr->LoadMesh(mesh);
	mr->SetRenderBackSide(false);
	mr->SetMeshColor(Vector3(0.0f, 1.0f, 0.0f));

	// Apply forward force
	RigidBody* rb = static_cast<RigidBody*>(entity->GetComponent(RigidBodyC));
	rb->SetVelocity(Vector3(0.0f, 0.0f, 50.0f));

	// Ball script data
	Ball* ball = static_cast<Ball*>(entity->GetComponent(BallC));
	ball->minYPos = -10.0f;

	// Initialize the ball entity
	entity->Initialize();

	++ballCounter;
	Logger::Get().Log("Balls spawned: " + std::to_string(ballCounter));
}
