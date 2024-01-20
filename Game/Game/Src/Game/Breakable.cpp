// @file: Breakable.cpp
//
// @brief: Cpp file for breakable component functionalities.

#include "stdafx.h"
#include "Breakable.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Particles.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CollisionSystem.h"

void Breakable::Initialize()
{
	// Load the mesh & its settings
	meshRenderer = static_cast<MeshRenderer*>(GetEntity()->GetComponent(MeshRendererC));
	meshRenderer->LoadMesh("Assets/Objects/Pyramid.obj");
	meshRenderer->SetRenderBackSide(false);
	meshRenderer->SetMeshColor(Vector3(0.0f, 0.8f, 1.0f));
	
	// Mark the collider as Ball collider
	boxC = static_cast<BoxCollider*>(GetEntity()->GetComponent(BoxColliderC));
	boxC->SetColliderTag(BREAKABLE);

	// On collision enter callback
	boxC->SetOnCollisionEnterCallback([this](Collider* collider) {
		// Break only if the ball hits it
		if (collider->GetColliderTag() == BALL)
			this->Break();
	});

	// Rigidbody settings
	RigidBody* rb = static_cast<RigidBody*>(entity->GetComponent(RigidBodyC));
	rb->resCoeff = 0.0f;  // inelastic object

	// Particles settings
	particles = static_cast<Particles*>(entity->GetComponent(ParticlesC));
	particles->SetParticleType(EXPLOSION);

	timeToDie = false;
}

void Breakable::Update(float deltaTime)
{
	if (timeToDie)
	{
		timeLeft -= (deltaTime / 1000.0f);
		if (timeLeft < 0)
			SceneManager::Get().GetActiveScene()->RemoveEntity(GetEntity());
	}
}

void Breakable::Break()
{
	if (timeToDie)
		return;

	// Hide the mesh
	meshRenderer->SetHideMesh(true);

	// Start particle effects
	particles->Emit(500);

	timeToDie = true;
}
