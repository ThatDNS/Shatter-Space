// @file: AimAssist.h
//
// @brief: Header file for AimAssist class.
// Responsible for generating a line predicting ball path.

#pragma once
#ifndef _AIM_ASSIST_H_
#define _AIM_ASSIST_H_

#include "Engine/Components/Component.h"

class Particles;
class BallSpawner;

class AimAssist : public Component
{
	Particles* particles = nullptr;
	BallSpawner* spawner = nullptr;

protected:
	// Protected destructor so that only Entity can delete it
	~AimAssist() = default;

	void Initialize() override;
	void Update(float) override;
	void Destroy() override { }

public:
	AimAssist() { type = AimAssistC; }
};

#endif // !_AIM_ASSIST_H_
