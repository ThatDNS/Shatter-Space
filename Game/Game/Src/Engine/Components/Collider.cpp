// @file: Collider.cpp
//
// @brief: Cpp file for Collider class. Creating this component causes
// the entity to be tracked by collision system.

#include "stdafx.h"
#include "Engine/Components/Collider.h"
#include "Engine/Systems/CollisionSystem.h"

Collider::Collider()
{
	CollisionSystem::Get().AddCollider(this);
}

Collider::~Collider()
{
	CollisionSystem::Get().RemoveCollider(this);
}
