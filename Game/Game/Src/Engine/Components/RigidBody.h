// @file: RigidBody.h
//
// @brief: Header file for RigidBody class, storing the data required for physics computations.

#pragma once
#ifndef _RIGID_BODY_H_
#define _RIGID_BODY_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"

class Collider;

class RigidBody : public Component
{
public:
	float mass = 1.0f;
	float drag = 0.01f;  // or friction

	Collider* collider = nullptr;

	Vector3 velocity{ 0.0f, 0.0f, 0.0f };
	// Instantaneous acceleration (i.e. it'll be applied only once)
	Vector3 instAcceleration{ 0.0f, 0.0f, 0.0f };

	RigidBody() { type = RigidBodyC; }

	void ApplyForce(const Vector3&);
	void SetVelocity(const Vector3&);

	void Initialize() override;
	void Update(float) override {}
	void Destroy() override {}
};

#endif // !_RIGID_BODY_H_
