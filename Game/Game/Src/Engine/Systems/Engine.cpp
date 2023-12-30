#include "stdafx.h"
#include "Engine/Systems/Engine.h"
#include "Engine/Core/Logger.h"
#include "Engine/Systems/SceneManager.h"

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
}

void Engine::Destroy()
{
	SceneManager::Get().Destroy();
}

void Engine::GameLoop()
{
	bool _isRunning = true;
	while (_isRunning)
	{
		// --------------------- Pre-update Phase ---------------------
		SceneManager::Get().PreUpdate();

		// --------------------- Update Phase ---------------------
		SceneManager::Get().Update();

		// --------------------- Post-update Phase ---------------------
		SceneManager::Get().PostUpdate();
	}
}
