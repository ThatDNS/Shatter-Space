// @file: CollisionSystem.cpp
//
// @brief: Cpp file for CollisionSystem, a singleton responsible for checking collisions.

#include "stdafx.h"
#include "Engine/Systems/CollisionSystem.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Algorithms/BVH.h"

void CollisionSystem::Initialize()
{
	bvhTree = new BVH();

	// CollisionSystem gets initialized after the SceneManager
	// So we can safely assume that initial colliders are present in the list
	std::vector<BoxCollider*> boxColliders;
	for (Collider* collider : colliders)
	{
		if (collider->GetColliderType() == BOX)
			boxColliders.push_back(static_cast<BoxCollider*>(collider));
	}
	Logger::Get().Log("Got " + std::to_string(boxColliders.size()) + " box colliders");
	bvhTree->BuildTree(boxColliders);
	Logger::Get().Log("Built BVH Tree!");
}

void CollisionSystem::Destroy()
{
	if (bvhTree != nullptr)
	{
		bvhTree->Destroy();
		delete bvhTree;
	}
	Logger::Get().Log("Destroyed BVH Tree!");
}

void CollisionSystem::AddCollider(Collider* collider)
{
	colliders.push_back(collider);
	Logger::Get().Log("Added a collider");
	// TODO: Update bvh tree here
}

void CollisionSystem::RemoveCollider(Collider* collider)
{
	colliders.remove(collider);
	Logger::Get().Log("Removed a collider");
	// TODO: Update bvh tree here
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

