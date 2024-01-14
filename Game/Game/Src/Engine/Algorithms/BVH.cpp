// @file: BVH.cpp
//
// @brief: Cpp file for the BVH (Bounding Volume Hierarchy) data structure implemetation class.
// It works only with box colliders as it uses AABBs.

#include "stdafx.h"
#include "BVH.h"

// --------------------------- Private member functions ---------------------------

bool BVH::CheckCollisions(BVHNode* node, const BoxCollider* collider) const
{
	// If the node does not intersect with box collider then no need of checking its child nodes
	if (node == nullptr || !node->boundingBox.Intersects(collider->boundingBox))
	{
		return false;
	}

	// For the leaf node, we check individual collisions with all the colliders
	if (node->IsLeaf())
	{
		for (const BoxCollider* leafC : node->colliders)
		{
			if (collider->boundingBox.Intersects(leafC->boundingBox))
			{
				// Collision detected!
				return true;
			}
		}
		return false;
	}

	// Collision happened with this BVH node so check child nodes
	return CheckCollisions(node->left, collider) || CheckCollisions(node->right, collider);
}

AABB BVH::GetEnclosingBoundingBox(const std::vector<BoxCollider*>& colliders) const
{
	Vector3 minC(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector3 maxC(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

	for (const BoxCollider* boxC : colliders)
	{
		minC.x = std::min(minC.x, boxC->boundingBox.minCoords.x);
		minC.y = std::min(minC.y, boxC->boundingBox.minCoords.y);
		minC.z = std::min(minC.z, boxC->boundingBox.minCoords.z);

		maxC.x = std::max(maxC.x, boxC->boundingBox.maxCoords.x);
		maxC.y = std::max(maxC.y, boxC->boundingBox.maxCoords.y);
		maxC.z = std::max(maxC.z, boxC->boundingBox.maxCoords.z);
	}

	return AABB(minC, maxC);
}

void BVH::SplitColliders(std::vector<BoxCollider*>& colliders, AABB& nodeBB, std::vector<BoxCollider*>& left, std::vector<BoxCollider*>& right)
{
	// Find the longest axis
	Vector3 extents = nodeBB.maxCoords - nodeBB.minCoords;
	// X: 0, Y: 1, Z: 2
	int longestAxis = (extents.x > extents.y) ? ((extents.x > extents.z) ? 0 : 2) : ((extents.y > extents.z) ? 1 : 2);

	// Sort colliders based on the min values on the longest axis
	std::sort(colliders.begin(), colliders.end(), [longestAxis](BoxCollider* a, BoxCollider* b) {
		return a->boundingBox.minCoords[longestAxis] < b->boundingBox.minCoords[longestAxis];
		});

	// Split colliders into 2 halfs
	size_t halfSize = colliders.size() / 2;
	left.assign(colliders.begin(), colliders.begin() + halfSize);
	right.assign(colliders.begin() + halfSize, colliders.end());
}

void BVH::Destroy(BVHNode* node)
{
	if (node == nullptr)
		return;

	// Traverse the tree to reach end of it
	Destroy(node->left);
	Destroy(node->right);

	// Clean up
	if (node->left != nullptr)
		delete node->left;
	if (node->right != nullptr)
		delete node->right;
}

// --------------------------- Public member functions ---------------------------

BVHNode* BVH::BuildTree(std::vector<BoxCollider*>& colliders)
{
	if (colliders.size() == 0)
	{
		return nullptr;
	}

	// Create a new node that contains all colliders
	AABB nodeBoundingBox = GetEnclosingBoundingBox(colliders);
	BVHNode* node = new BVHNode(nodeBoundingBox);

	// If number of colliders is less, make it leaf
	if (colliders.size() <= 4)
	{
		node->colliders = colliders;
	}
	else
	{
		// Split colliders into two groups along the longest axis
		std::vector<BoxCollider*> leftColliders, rightColliders;
		SplitColliders(colliders, nodeBoundingBox, leftColliders, rightColliders);

		// Recursively build left and right child nodes
		node->left = BuildTree(leftColliders);
		node->left = BuildTree(rightColliders);
	}

	return node;
}

void BVH::Destroy()
{
	Destroy(root);
}

bool BVH::CheckCollisions(const BoxCollider* boxCollider) const
{
	return CheckCollisions(root, boxCollider);
}
