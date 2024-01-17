// @file: Player.cpp
//
// @brief: Cpp file for player functionalities.

#include "stdafx.h"

#include "Game/Player.h"
#include "App/app.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Sprite.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Particles.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Systems/CollisionSystem.h"

void Player::Initialize()
{
	// Cache components
	collider = static_cast<BoxCollider*>(GetEntity()->GetComponent(BoxColliderC));
	rigidBody = static_cast<RigidBody*>(GetEntity()->GetComponent(RigidBodyC));
	particles = static_cast<Particles*>(GetEntity()->GetComponent(ParticlesC));

	// Attach camera to the player
	RenderSystem::Get().AttachCamera(GetEntity());
}

void Player::Update(float deltaTime)
{
	Move(deltaTime);
}

void Player::Move(float deltaTime)
{
	Vector3 moveVector{ 0.0f, 0.0f, 0.0f };
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		++moveVector.x;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		--moveVector.x;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		++moveVector.y;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		--moveVector.y;
	}
	moveVector.Normalize();
	if (moveVector.Magnitude() == 0)
		return;

	moveVector = moveVector * (moveSpeed * deltaTime / 100.0f);

	// Move the entity
	rigidBody->ApplyForce(moveVector);
	// Rotate the entity in direction of motion
	GetEntity()->CartesianRotationZ(moveVector, collider, rotateSpeed * deltaTime / 100.0f);
	// Particles
	particles->Emit(1, -moveVector);
}
