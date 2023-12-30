#include "stdafx.h"

#include "Engine/Systems/Engine.h"
#include "Engine/Core/Logger.h"

#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/RenderSystem.h"

extern void Engine_Register();

void Engine::Initialize()
{
	Engine_Register();

	// Setup the logger
	Logger::Get().SetLogfile();

	// Load the managers
	SceneManager::Get().Load();

	// Initialize the managers
	SceneManager::Get().Initialize();
	RenderSystem::Get().Initialize();
}

void Engine::Destroy()
{
	SceneManager::Get().Destroy();
}

void Engine::Update(float deltaTime)
{
	// --------------------- Pre-update Phase ---------------------
	SceneManager::Get().PreUpdate();

	// --------------------- Update Phase ---------------------
	SceneManager::Get().Update();

	// --------------------- Post-update Phase ---------------------
	SceneManager::Get().PostUpdate();
}

void Engine::Render()
{
	RenderSystem::Get().Render();
}
