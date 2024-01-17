#include "stdafx.h"

#include "Engine/Systems/Engine.h"
#include "Engine/Core/Logger.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Systems/CollisionSystem.h"
#include "Engine/Systems/PhysicsSystem.h"
#include "Engine/Pools/EntityPool.h"

void Engine::Wakeup()
{
	// Setup the logger
	Logger::Get().SetLogfile();
}

void Engine::Initialize()
{
	timeElapsed = 0.0f;

	// Initialize the managers
	SceneManager::Get().Initialize();
	RenderSystem::Get().Initialize();
	CollisionSystem::Get().Initialize();
}

void Engine::Destroy()
{
	SceneManager::Get().Destroy();
	CollisionSystem::Get().Destroy();
}

void Engine::Update(float deltaTime)
{
	timeElapsed += deltaTime;

	// --------------------- Pre-update Phase ---------------------
	SceneManager::Get().PreUpdate();

	// --------------------- Update Phase ---------------------
	SceneManager::Get().Update(deltaTime);
	RenderSystem::Get().Update(deltaTime);
	PhysicsSystem::Get().Update(deltaTime);

	// --------------------- Post-update Phase ---------------------
	SceneManager::Get().PostUpdate();

	// Don't need to update collision system every frame
	// Update it only once every x seconds
	if (timeElapsed >= COLLISION_SYSTEM_UPDATE_TIME)
	{
		CollisionSystem::Get().Update();
		timeElapsed = 0.0f;
	}
}

void Engine::Render()
{
	// Render the game
	RenderSystem::Get().Render();
}
