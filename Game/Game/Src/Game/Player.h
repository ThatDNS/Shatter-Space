// @file: Player.h
//
// @brief: Header file for player functionalities.

#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Engine/Components/Component.h"

class Player : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

	float moveSpeed = 1.0f;

	void Move(float);

public:
	void Initialize() override;
	void Load(json::JSON& jsonData);
	void Update(float) override;
	void Destroy() override {}

	void SetMoveSpeed(float value) { moveSpeed = value; }
};

#endif // !_PLAYER_H_
