// @file: Ball.h
//
// @brief: Header file for ball functionalities.

#pragma once
#ifndef _BALL_H_
#define _BALL_H_

#include "Engine/Components/Component.h"

class Ball : public Component
{
public:
	Ball() { type = BallC; }

	void Initialize() override;
	void Update(float) override {}
	void Destroy() override {}

	friend class BallSpawner;
};

#endif // !_BALL_H_
