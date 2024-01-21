// @file: StarsController.h
//
// @brief: Header file for StarsController class responsible for controlling stars particles.

#pragma once
#ifndef _STARS_CONTROLLER_H_
#define _STARS_CONTROLLER_H_

#include "Engine/Components/Component.h"

class Particles;

class StarsController : public Component
{
	Particles* particles = nullptr;

public:
	StarsController() { type = StarsControllerC; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_STARS_CONTROLLER_H_
