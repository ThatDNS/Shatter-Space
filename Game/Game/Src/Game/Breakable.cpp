// @file: Breakable.cpp
//
// @brief: Cpp file for breakable component functionalities.

#include "stdafx.h"
#include "Game/Breakable.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Particles.h"
#include "Engine/Math/Random.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CollisionSystem.h"
#include "Game/SelfDestruct.h"
#include "Game/UIManager.h"

void Breakable::Initialize()
{
	// Find the UIManager
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(UIManagerC);
	if (match.size() == 0)
		Logger::Get().Log("Breakable could not find UI Manager", ERROR_LOG);
	else
		uiManager = static_cast<UIManager*>(match.front()->GetComponent(UIManagerC));

	// Intentionally written without extension.
	// Names of breakable parts of this mesh have just indices appended to this filename.
	std::string meshObjFile = "Assets/Objects/Breakable/";
	if (breakableType == BreakableType::Pyramid)
	{
		meshObjFile += "Pyramid";
		numPieces = 4;
	}
	else if (breakableType == BreakableType::Plane)
	{
		meshObjFile += "Plane";
		numPieces = 8;
	}

	// Load the mesh & its settings
	meshRenderer = static_cast<MeshRenderer*>(GetEntity()->GetComponent(MeshRendererC));
	meshRenderer->LoadMesh(meshObjFile + ".obj");
	meshRenderer->SetRenderBackSide(false);
	meshRenderer->SetMeshColor(Vector3(0.0f, 0.8f, 1.0f));

	// Load broken mesh pieces
	brokenPieces.clear();
	for (size_t i = 0; i < numPieces; i++)
	{
		Mesh mesh;
		mesh.LoadFromObjectFile(meshObjFile + "_" + std::to_string(i + 1) + ".obj");
		if (mesh.faces.size() > 0)
			brokenPieces.push_back(mesh);
	}
	
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
	// Blue to Turquoise
	particles->SetParticleColors(Vector3(0.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));

	timeToDie = false;
	timeLeft = 1.0f;
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

	// Spawn broken pieces
	for (Mesh& mesh : brokenPieces)
	{
		SpawnBrokenPieces(mesh);
	}

	// Start particle effects
	particles->Emit(50);

	// Update the UI
	UIBuffer score;
	Vector3 position = GetEntity()->GetTransform().position;
	score.x = position.x;
	score.y = position.y + 3.0f;
	score.z = position.z;
	score.project = true;
	score.timeRemaining = 1.0f;
	score.text = "+2";
	uiManager->ScheduleRender(score);
	uiManager->IncreaseBalls(2);

	timeToDie = true;
}

void Breakable::SpawnBrokenPieces(Mesh& mesh)
{
	Scene* scene = SceneManager::Get().GetActiveScene();
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, RigidBodyC, SelfDestructC });
	entity->SetName("BrokenPiece");

	Vector3 randomFactor{ Random::Get().Float() * 2.0f - 1.0f, Random::Get().Float() * 2.0f - 1.0f, 0.0f };
	entity->GetTransform().position = GetEntity()->GetTransform().position + randomFactor;

	// Load the mesh
	MeshRenderer* mr = static_cast<MeshRenderer*>(entity->GetComponent(MeshRendererC));
	mr->LoadMesh(mesh);
	mr->SetRenderBackSide(false);
	mr->SetMeshColor(meshRenderer->GetMeshColor());

	// Apply outward velocity
	RigidBody* rb = static_cast<RigidBody*>(entity->GetComponent(RigidBodyC));
	Vector3 velocity{ Random::Get().Float() * 0.5f, Random::Get().Float() * 0.5f, Random::Get().Float() };
	velocity.Normalize();
	velocity *= 20.0f;
	rb->SetVelocity(velocity);

	// Self destuct data
	SelfDestruct* sd = static_cast<SelfDestruct*>(entity->GetComponent(SelfDestructC));
	sd->SetBound(Vector3(0.0f, -20.0f, 0.0f));  // Currently it checks only Y

	// Initialize
	entity->Initialize();
}
