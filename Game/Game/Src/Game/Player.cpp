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
#include "Engine/Math/Vector3.h"

IMPLEMENT_DYNAMIC_CLASS(Player)

void Player::Initialize()
{
}

void Player::Update(float deltaTime)
{
	Move(deltaTime);

	// Rotate
	Transform& transform = GetEntity()->GetTransform();
	transform.Rotate(Vector3(0, 0.01f, 0.015f));
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
		++moveVector.z;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		--moveVector.z;
	}

	moveVector.Normalize();
	moveVector = moveVector * (moveSpeed * deltaTime / 100.0f);

	Transform& transform = GetEntity()->GetTransform();
	Vector3 newPosition = transform.position + moveVector;

	// Translate the transform
	transform.Translate(moveVector);
}
