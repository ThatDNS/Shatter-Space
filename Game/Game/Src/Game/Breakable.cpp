// @file: Breakable.cpp
//
// @brief: Cpp file for breakable component functionalities.

#include "stdafx.h"
#include "App/app.h"
#include "Game/Breakable.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Particles.h"
#include "Engine/Math/Random.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Systems/CollisionSystem.h"
#include "Game/SelfDestruct.h"
#include "Game/UIManager.h"
#include "Game/DoorOpener.h"

void Breakable::LoadMeshes(std::string& meshObjFile, size_t meshPieces)
{
	meshRenderer->LoadMesh(meshObjFile + ".obj");
	meshRenderer->SetRenderBackSide(false);
	if (breakableType == BreakableType::Star)
		meshRenderer->SetMeshColor(Vector3(1.0f, 1.0f, 0.0f));
	else if (breakableType == BreakableType::Plane)
		meshRenderer->SetMeshColor(Vector3(0.0f, 0.3f, 1.0f));
	else
		meshRenderer->SetMeshColor(Vector3(0.0f, 0.8f, 1.0f));

	// Load broken mesh pieces
	brokenPieces.clear();
	for (size_t i = 0; i < meshPieces; i++)
	{
		Mesh mesh;
		mesh.LoadFromObjectFile(meshObjFile + "_" + std::to_string(i + 1) + ".obj");
		if (mesh.faces.size() > 0)
			brokenPieces.push_back(mesh);
	}
}

void Breakable::Initialize()
{
	// Find the UIManager
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(UIManagerC);
	if (match.size() == 0)
		Logger::Get().Log("Breakable could not find UI Manager", ERROR_LOG);
	else
		uiManager = static_cast<UIManager*>(match.front()->GetComponent(UIManagerC));

	// Cache components
	meshRenderer = static_cast<MeshRenderer*>(GetEntity()->GetComponent(MeshRendererC));
	particles = static_cast<Particles*>(GetEntity()->GetComponent(ParticlesC));
	rigidBody = static_cast<RigidBody*>(GetEntity()->GetComponent(RigidBodyC));

	// Intentionally written without extension.
	// Names of breakable parts of this mesh have just indices appended to this filename.
	std::string meshObjFile = "Assets/Objects/Breakable/";
	size_t numPieces = 0;
	if (breakableType == BreakableType::Pyramid)
	{
		meshObjFile += "Pyramid";
		numPieces = 4;
		_score = PYRAMID_SCORE;
	}
	else if (breakableType == BreakableType::Plane)
	{
		meshObjFile += "Plane";
		numPieces = 8;
		_score = PLANE_SCORE;
	}
	else if (breakableType == BreakableType::Star)
	{
		meshObjFile += "Star";
		numPieces = 7;
		_score = STAR_SCORE;
	}
	
	// ----------------------- Mesh Renderer -----------------------
	// Load the meshes
	LoadMeshes(meshObjFile, numPieces);

	// ----------------------- Box Collider -----------------------
	BoxCollider* boxC = static_cast<BoxCollider*>(GetEntity()->GetComponent(BoxColliderC));
	// Mark the collider as Ball collider
	boxC->SetColliderTag(BREAKABLE);

	// On collision enter callback
	boxC->SetOnCollisionEnterCallback([this](Collider* collider) {
		// Break only if the ball hits it
		if (collider->GetColliderTag() == BALL)
			this->Break();
	});

	// ----------------------- RigidBody & Particles -----------------------
	// Rigidbody settings
	rigidBody->resCoeff = 0.0f;  // inelastic object

	if (breakableType == BreakableType::Plane)
	{
		// Plane shouldn't be affected by gravity
		rigidBody->applyGravity = false;
		rigidBody->SetVelocity(Vector3{ 0.0f, 0.0f, 0.0f });

		// Planes don't use particles
	}
	else if (breakableType == BreakableType::Pyramid)
	{
		rigidBody->applyGravity = true;
		rigidBody->SetVelocity(Vector3{ 0.0f, 0.0f, 0.0f });

		particles->SetPositionOffset(Vector3(0.0f, 2.5f, 0.0f));
		particles->SetParticleType(EXPLOSION);
		// Blue to Turquoise
		particles->SetParticleColors(Vector3(0.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	}
	else if (breakableType == BreakableType::Star)
	{
		rigidBody->applyGravity = true;
		// Shooting star definitely has to fall!
		float xVel = 20.0f;
		if (GetEntity()->GetTransform().position.x > 0)
			xVel = -xVel;
		rigidBody->SetVelocity(Vector3{ xVel, -5.0f, 0.0f });

		particles->SetPositionOffset(Vector3(-0.5f, -0.5f, 0.0f));
		particles->SetParticleType(PROPULSION);
		// Yellow to white
		particles->SetParticleColors(Vector3{ 1.0f, 1.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
	}

	theta = 0.0f;
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
	else
	{
		if (breakableType == BreakableType::Star)
		{
			// Rotate the stars
			float rotation = GetEntity()->GetTransform().rotation.z + (deltaTime / 500.0f);
			GetEntity()->GetTransform().rotation.z = std::fmod(rotation, 2.0f * PI);

			// Move trail
			particles->Emit(1, -rigidBody->velocity);
		}
		else if (breakableType == BreakableType::Plane)
		{
			// Check if it got very close to the camera
			Vector3& cameraPos = RenderSystem::Get().GetCameraPosition();
			Vector3& position = GetEntity()->GetTransform().position;
			if ((std::abs(position.z - std::abs(cameraPos.z)) < 2.0f) && (std::abs(position.y + 6.0 - std::abs(cameraPos.y)) < 1.0f))
			{
				UIBuffer damage;
				damage.position.x = APP_VIRTUAL_WIDTH / 2 - 70;
				damage.position.y = APP_VIRTUAL_HEIGHT - 60;
				damage.project = false;
				damage.timeRemaining = 2.0f;
				damage.text = "Took Damage! (-5)";
				damage.color = Vector3(1.0f, 0.0f, 0.0f);
				uiManager->ScheduleRender(damage);
				uiManager->DecreaseBalls(5);

				Break(false);
			}

			// Move if it should
			if (moveVertically)
			{
				SHMMovement(deltaTime);
			}
		}
	}
}

void Breakable::SHMMovement(float deltaTime)
{
	theta = std::fmod(theta + deltaTime / 1000.0f, 2 * PI);
	float position = amplitude * std::sinf(theta);

	// 6.0f is mesh renderer's offset for plane. Wish I had the time to fix this
	GetEntity()->GetTransform().position.y = position - 6.0f;
}

void Breakable::Break(float updateScore)
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
	if (breakableType == Pyramid)
		particles->Emit(64);

	// Update the UI
	if (updateScore && _score > 0)
	{
		UIBuffer score;
		Vector3 position = GetEntity()->GetTransform().position;
		score.position = position;
		score.position.y += 3.0f;
		score.project = true;
		score.timeRemaining = 1.0f;
		score.text = "+" + std::to_string(_score);
		score.color = Vector3(0.0f, 1.0f, 0.0f);
		uiManager->ScheduleRender(score);
		uiManager->IncreaseBalls(_score);

		if (breakableType == BreakableType::Star)
		{
			UIBuffer achievement;
			achievement.position.x = APP_VIRTUAL_WIDTH / 2 - 90;
			achievement.position.y = APP_VIRTUAL_HEIGHT - 80;
			achievement.project = false;
			achievement.timeRemaining = 2.0f;
			achievement.text = "Shot the Shooting Star! (+5)";
			achievement.color = Vector3(1.0f, 1.0f, 0.0f);
			uiManager->ScheduleRender(achievement);
		}
	}

	// Indicate the door openers to open
	for (DoorOpener* door : doorOpeners)
	{
		door->SetOpenDoor(true);
	}

	if (breakableType == Plane)
	{
		std::string soundFile = glassBreakSound + "big.wav";
		App::PlaySound(soundFile.c_str());
	}
	else
	{
		int randomInt = static_cast<int>(Random::Get().Float() * 3.9f);
		std::string soundFile = glassBreakSound + std::to_string(randomInt + 1) + ".wav";
		App::PlaySound(soundFile.c_str());
	}
	
	timeToDie = true;
}

void Breakable::SpawnBrokenPieces(Mesh& mesh)
{
	Scene* scene = SceneManager::Get().GetActiveScene();
	Entity* entity = scene->CreateEntity(std::vector<ComponentType>{ MeshRendererC, RigidBodyC, SelfDestructC });
	entity->SetName("BrokenPiece");

	Transform& transform = GetEntity()->GetTransform();
	Vector3 randomFactor{ 0.0f, Random::Get().Float() * 2.0f - 1.0f, 0.0f };
	entity->GetTransform().position = transform.position + randomFactor;
	entity->GetTransform().rotation = transform.rotation;
	entity->GetTransform().scale = transform.scale;

	// Load the mesh
	MeshRenderer* mr = static_cast<MeshRenderer*>(entity->GetComponent(MeshRendererC));
	mr->LoadMesh(mesh);
	mr->SetRenderBackSide(false);
	mr->SetMeshColor(meshRenderer->GetMeshColor());

	// Apply outward velocity
	RigidBody* rb = static_cast<RigidBody*>(entity->GetComponent(RigidBodyC));
	Vector3 velocity{ (Random::Get().Float() * 2.0f - 1.0f) * 0.1f, Random::Get().Float() * 0.1f, Random::Get().Float() };
	velocity.Normalize();
	velocity *= 20.0f;
	rb->SetVelocity(velocity);

	// Self destuct data
	SelfDestruct* sd = static_cast<SelfDestruct*>(entity->GetComponent(SelfDestructC));
	sd->SetBound(Vector3(0.0f, -20.0f, 0.0f));  // Currently it checks only Y

	// Initialize
	entity->Initialize();
}
