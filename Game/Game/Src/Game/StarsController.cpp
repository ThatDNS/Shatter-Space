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
	particles = static_cast<Particles*>(GetEntity()->GetComponent(ParticlesC));
	particles->SetParticleType(STARS);
	particles->SetParticleColors(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
}

void StarsController::Update(float deltaTime)
{
	particles->Emit(5);
}
