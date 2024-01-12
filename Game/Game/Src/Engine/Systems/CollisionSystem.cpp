// @file: CollisionSystem.cpp
//
// @brief: Cpp file for CollisionSystem, a singleton responsible for checking collisions.

#include "stdafx.h"
#include "Engine/Systems/CollisionSystem.h"
#include "Engine/Components/Entity.h"
#include "Engine/Math/Vector3.h"

void CollisionSystem::AddCollider(Collider* collider)
{
	colliders.push_back(collider);
}

void CollisionSystem::RemoveCollider(Collider* collider)
{
	colliders.remove(collider);
}

bool CollisionSystem::IsSpaceEmpty(Entity* entity, Vector3& target)
{
	return true;
}

bool CollisionSystem::IsSpaceEmpty(float objRadius, Vector3& target)
{
	return true;
}

Collider* CollisionSystem::CheckIfCollided(Entity* entity, ColliderTag _colliderTag)
{
	return nullptr;
}

