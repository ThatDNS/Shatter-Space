// @file: Breakable.h
//
// @brief: Header file for breakable component functionalities.

#pragma once
#ifndef _BREAKABLE_H_
#define _BREAKABLE_H_

#include "Engine/Components/Component.h"

class BoxCollider;

class Breakable : public Component
{
	// Cached components
	BoxCollider* boxC = nullptr;

public:
	Breakable() { type = BreakableC; }
	
	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_BREAKABLE_H_
