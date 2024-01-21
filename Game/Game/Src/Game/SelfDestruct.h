// @file: SelfDestruct.h
//
// @brief: Header file for SelfDestruct class which deletes an entity if it goes beyond the specified bound.

#pragma once
#ifndef _SELF_DESTRUCT_H_
#define _SELF_DESTRUCT_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"

class Entity;

class SelfDestruct : public Component
{
	// Self destructs if it goes below this position
	Vector3 bound{ 0.0f, -20.0f, 10.0f };

	Entity* playerEntity = nullptr;

public:
	SelfDestruct() { type = SelfDestructC; }

	void SetBound(Vector3& b) { bound = b; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_SELF_DESTRUCT_H_
