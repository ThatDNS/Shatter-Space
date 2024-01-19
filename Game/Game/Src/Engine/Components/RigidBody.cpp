// @file: RigidBody.cpp
//
// @brief: Cpp file for RigidBody class, storing the data required for physics computations.

#include "stdafx.h"
#include "RigidBody.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Core/Logger.h"

void RigidBody::Initialize()
{
	// Check if it has collider. If yes, cache it
	Component* component = GetEntity()->GetComponent(BoxColliderC);
	if (component != nullptr)
		collider = static_cast<BoxCollider*>(component);
}

void RigidBody::ApplyForce(const Vector3& force)
{
	// Force causes acceleration
	instAcceleration += force / mass;
}

void RigidBody::SetVelocity(const Vector3& v)
{
	velocity = v;
}
