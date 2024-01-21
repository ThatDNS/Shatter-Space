// @file: AimAssist.cpp
//
// @brief: Cpp file for AimAssist class.
// Responsible for generating a line predicting ball path.

#include "stdafx.h"
#include "App/app.h"
#include "Game/AimAssist.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Particles.h"
#include "Game/BallSpawner.h"

void AimAssist::Initialize()
{
	// Get BallSpawner
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(BallSpawnerC);
	if (match.size() == 0)
	{
		Logger::Get().Log("AimAssist could not find BallSpawner", ERROR_LOG);
	}
	else
	{
		spawner = static_cast<BallSpawner*>(match.front()->GetComponent(BallSpawnerC));
	}

	// Get the particles components
	particles = static_cast<Particles*>(GetEntity()->GetComponent(ParticlesC));
	particles->SetParticleType(AIM_ASSIST);
	particles->SetParticleColors(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
}

void AimAssist::Update(float deltaTime)
{
	Vector3 particlePosition = spawner->GetEntity()->GetTransform().position;
	particlePosition.z += 5.0f;  // start a little bit in front of spawner

	// Mouse position becomes the velocity direction for Aim assist lines
	particles->Emit(10, Vector3{ 0.0f, 0.0f, 0.0f }, particlePosition);
}
