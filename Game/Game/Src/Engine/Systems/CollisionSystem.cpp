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
	BuildNewBVHTree();
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
	{
		if (treeUpdateCount >= MAX_TREE_UPDATE_ITERS)
		{
			// BVH tree has been rebuilt a lot of times. In each rebuilt, its AABBs are adjusted. These
			// adjustments could easily lead to inefficiencies over time. Hence, its important to recreate
			// a fully-efficient BVH tree every once a while.
			bvhTree->Destroy();
			BuildNewBVHTree();

			treeUpdateCount = 0;
		}
		else
		{
			bvhTree->RebuildTree();

			++treeUpdateCount;
		}
	}
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

void CollisionSystem::BuildNewBVHTree()
{
	std::vector<BoxCollider*> boxColliders;
	for (Collider* collider : colliders)
	{
		if (collider->GetColliderType() == BOX)
			boxColliders.push_back(static_cast<BoxCollider*>(collider));
	}
	bvhTree->BuildTree(boxColliders);
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

Vector3 CollisionSystem::GetCollisionNormal(Collider* collider)
{
	if (collider->GetColliderType() == BOX)
	{
		return bvhTree->GetCollisionNormal(static_cast<BoxCollider*>(collider));
	}

	// Not supporting any other collisions yet
	return Vector3(0.0f, 0.0f, 0.0f);
}
