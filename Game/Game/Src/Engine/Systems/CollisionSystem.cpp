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
	bvhTree->BuildTree(boxColliders);
}

void CollisionSystem::Update()
{
	bool updateTree = false;
	// Check if any box colliders got updated. If yes, we need to update the BVH tree
	for (Collider* collider : colliders)
	{
		if (collider->GetColliderType() == BOX && collider->gotUpdated)
		{
			collider->gotUpdated = false;
			updateTree = true;
		}
	}

	// Update BVH Tree
	if (updateTree)
		bvhTree->RebuildTree();
}

void CollisionSystem::Destroy()
{
	if (bvhTree != nullptr)
	{
		bvhTree->Destroy();
		delete bvhTree;
	}
}

void CollisionSystem::AddCollider(Collider* collider)
{
	colliders.push_back(collider);
}

void CollisionSystem::RemoveCollider(Collider* collider)
{
	colliders.remove(collider);
}

bool CollisionSystem::CheckCollision(Collider* collider)
{
	if (collider->GetColliderType() == BOX)
	{
		return bvhTree->CheckCollisions(static_cast<BoxCollider*>(collider));
	}
	
	// Not supporting any other collisions yet
	return false;
}
