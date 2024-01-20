// @file: BVH.h
//
// @brief: Header file for the BVH (Bounding Volume Hierarchy) data structure implemetation class.
// It works only with box colliders as it uses AABBs.

#pragma once
#ifndef _BVH_H_
#define _BVH_H_

#include "Engine/Algorithms/AABB.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Math/Vector3.h"

// Node of a BVH tree
class BVHNode {
public:
	// AABB of the BVH node
	AABB boundingBox;

	// Colliders part of this BVH node
	std::vector<BoxCollider*> colliders;

	BVHNode* parent = nullptr;
	BVHNode* left = nullptr;
	BVHNode* right = nullptr;

	BVHNode(const AABB& aabb) : boundingBox(aabb) {}

	bool IsLeaf() const
	{
		return (left == nullptr && right == nullptr);
	}
};

/**
 * @class BVH
 *
 * BVH (Bounding Volume Hierarchy) has been used as an efficient solution for collison detection.
 * Refer: https://en.wikipedia.org/wiki/Bounding_volume_hierarchy
 */
class BVH
{
	friend class TestBVH;

private:
	BVHNode* root;

	/**
	 * @brief Recursively build BVH tree via top-down method.
	 */
	BVHNode* BuildTreeInternal(std::vector<BoxCollider*>& colliders);

	/**
	 * @brief Recursively check for collision.
	 */
	BoxCollider* CheckCollisions(BVHNode* node, BoxCollider* collider, Vector3& normal, ColliderTag colliderTag) const;

	/**
	 * @brief Compute the AABB enclosing a number of AABBs.
	 */
	AABB GetEnclosingBoundingBox(const std::vector<BoxCollider*>& colliders) const;

	/**
	 * @brief Split a vector containing colliders into 2 vectors.
	 * The split is made along the longest axis
	 */
	void SplitColliders(std::vector<BoxCollider*>& colliders, AABB& nodeBB, std::vector<BoxCollider*>& left, std::vector<BoxCollider*>& right);

	/**
	 * @brief Recursively re-build the tree using existign colliders.
	 */
	void BVH::RebuildTree(BVHNode* node);

	/**
	 * @brief Recursively delete all nodes of the tree.
	 */
	void Destroy(BVHNode* node);

public:
	/**
	 * @brief Recursively build BVH tree via top-down method.
	 */
	void BuildTree(std::vector<BoxCollider*>& colliders);

	/**
	 * @brief Empty the tree.
	 */
	void Destroy();

	/**
	 * @brief Check if anything collided with the input collider.
	 */
	BoxCollider* CheckCollisions(BoxCollider* boxCollider, ColliderTag colliderTag = GENERIC) const;

	/**
	 * @brief Get normal vector to the collision plane.
	 */
	Vector3 GetCollisionNormal(BoxCollider* boxCollider, ColliderTag colliderTag = GENERIC) const;

	/**
	 * @brief Recursively re-build the tree using existign colliders.
	 * Useful if the colliders have changed.
	 */
	void BVH::RebuildTree();
};

#endif // !_BVH_H_
