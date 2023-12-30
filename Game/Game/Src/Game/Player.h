// @file: Player.h
//
// @brief: Header file for player functionalities.

#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Engine/Components/Component.h"

class Transform;
class Sprite;

class Player : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

	float moveSpeed = 10.0f;

	// Cached components
	Transform* transform;
	Sprite* sprite;

	void Move(float);

public:
	void Initialize() override;
	void Load(json::JSON& jsonData);
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_PLAYER_H_
