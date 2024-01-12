// @file: Player.h
//
// @brief: Header file for player functionalities.

#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Engine/Components/Component.h"

class Player : public Component
{
	float moveSpeed = 1.0f;

	void Move(float);

public:
	Player() { type = PlayerC; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}

	void SetMoveSpeed(float value) { moveSpeed = value; }
};

#endif // !_PLAYER_H_
