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
	if (!collidersAddedRemoved)
	{
		// Check if any box colliders got updated. If yes, we need to update the BVH tree
		for (Collider* collider : colliders)
		{
			if (collider->GetColliderType() == BOX && collider->gotUpdated)
			{
				collider->gotUpdated = false;
				updateTree = true;
			}
		}
	}

	// Update BVH Tree
	if (collidersAddedRemoved || updateTree)
	{
		if (collidersAddedRemoved || (treeUpdateCount >= MAX_TREE_UPDATE_ITERS))
		{
			// Either new colliders got added / removed or BVH tree has been rebuilt a lot of times. 
			// 1. When a new collider gets added / removed, we can add / remove just it from the tree instead of
			//    building the entire tree again. However, in 1 frame update, multiple colliders can be added / removed.
			//    The BVH tree would be changed and its nodes would be recallibrated for each new collider.
			//    It seems better to just build a new BVH tree.
			// 2. In each rebuilt, its AABBs are adjusted. These adjustments could easily lead to inefficiencies
			//    over time. Hence, its important to recreate a fully-efficient BVH tree every once a while.
			bvhTree->Destroy();
			BuildNewBVHTree();

			treeUpdateCount = 0;
			collidersAddedRemoved = false;
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
	collidersAddedRemoved = true;
}

void CollisionSystem::RemoveCollider(Collider* collider)
{
	colliders.remove(collider);
	collidersAddedRemoved = true;
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

Collider* CollisionSystem::CheckCollision(Collider* collider, ColliderTag colliderTag)
{
	if (collider->GetColliderType() == BOX)
	{
		return bvhTree->CheckCollisions(static_cast<BoxCollider*>(collider), colliderTag);
	}
	
	// Not supporting any other collisions yet
	return nullptr;
}

Vector3 CollisionSystem::GetCollisionNormal(Collider* collider, ColliderTag colliderTag)
{
	if (collider->GetColliderType() == BOX)
	{
		return bvhTree->GetCollisionNormal(static_cast<BoxCollider*>(collider), colliderTag);
	}

	// Not supporting any other collisions yet
	return Vector3(0.0f, 0.0f, 0.0f);
}
