// @file: Player.h
//
// @brief: Header file for player functionalities.

#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Engine/Components/Component.h"

class BoxCollider;

class Player : public Component
{
	float moveSpeed = 1.0f;
	float rotateSpeed = 0.5f;

	// Cache the collider
	BoxCollider* collider = nullptr;

	void Move(float);
	void Rotate(float);

public:
	Player() { type = PlayerC; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}

	void SetMoveSpeed(float value) { moveSpeed = value; }
};

#endif // !_PLAYER_H_
