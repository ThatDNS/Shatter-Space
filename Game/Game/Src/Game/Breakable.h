// @file: Breakable.h
//
// @brief: Header file for breakable component functionalities.

#pragma once
#ifndef _BREAKABLE_H_
#define _BREAKABLE_H_

#include "Engine/Components/Component.h"

class MeshRenderer;
class BoxCollider;
class Particles;

class Breakable : public Component
{
	// Cached components
	MeshRenderer* meshRenderer = nullptr;
	BoxCollider* boxC = nullptr;
	Particles* particles = nullptr;

	// Delay in deleting the object
	bool timeToDie = false;
	float timeLeft = 1.0;

public:
	Breakable() { type = BreakableC; }
	
	void Break();

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_BREAKABLE_H_
