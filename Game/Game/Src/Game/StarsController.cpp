// @file: StarsController.cpp
//
// @brief: Cpp file for StarsController class responsible for controlling stars particles.

#include "stdafx.h"
#include "Game/StarsController.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Particles.h"

void StarsController::Initialize()
{
	// Find the particle system. Ensure its set to STARS
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(ParticlesC);
	if (match.size() == 0)
	{
		Logger::Get().Log("Stars controller could not find particles", ERROR_LOG);
	}
	else
	{
		particles = static_cast<Particles*>(match.front()->GetComponent(ParticlesC));
		particles->SetParticleType(STARS);
	}
}

void StarsController::Update(float deltaTime)
{
	particles->Emit(static_cast<int>(deltaTime / 4.0f));
}
