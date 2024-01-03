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
	Component* spriteComponent = GetEntity()->GetComponent("Sprite");
	if (spriteComponent == nullptr)
	{
		Logger::Get().Log("Player does not have sprite", ERROR_LOG);
		return;
	}
	sprite = static_cast<Sprite*>(spriteComponent);
	transform = GetEntity()->GetTransform();
}

void Player::Load(json::JSON& playerJSON)
{
	if (playerJSON.hasKey("MoveSpeed"))
	{
		moveSpeed = (float)playerJSON["MoveSpeed"].ToFloat();
	}
}

void Player::Update(float deltaTime)
{
	Move(deltaTime);
}

void Player::Move(float deltaTime)
{
	int verticalMove = 0;
	int horizontalMove = 0;
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		sprite->SetAnimation(ANIM_RIGHT);
		++horizontalMove;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		sprite->SetAnimation(ANIM_LEFT);
		--horizontalMove;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		sprite->SetAnimation(ANIM_FORWARDS);
		++verticalMove;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		sprite->SetAnimation(ANIM_BACKWARDS);
		--verticalMove;
	}
	/*if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		testSprite->SetScale(testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		testSprite->SetScale(testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		testSprite->SetAnimation(-1);
	}*/

	Vector3 moveVector{ (float)horizontalMove, (float)verticalMove, 0.0f };
	moveVector.Normalize();
	moveVector = moveVector * (moveSpeed * deltaTime / 100.0f);
	Vector3 newPosition = transform->position + moveVector;

	// Translate the transform
	transform->Translate(moveVector);
}
