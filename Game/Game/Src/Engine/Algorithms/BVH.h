// @file: BVH.h
//
// @brief: Header file for the BVH (Bounding Volume Hierarchy) data structure implemetation class.
// It works only with box colliders as it uses AABBs.

#pragma once
#ifndef _BVH_H_
#define _BVH_H_

#include "Engine/Algorithms/AABB.h"
#include "Engine/Components/BoxCollider.h"

// Node of a BVH tree
class BVHNode {
public:
	// AABB of the BVH node
	AABB boundingBox;

	// Colliders part of this BVH node
	std::vector<BoxCollider*> colliders;

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
 * BVH (Bounding Volume Hierarchy) has been used as a scalable solution for collison detection.
 * Refer: https://en.wikipedia.org/wiki/Bounding_volume_hierarchy
 */
class BVH
{
private:
	BVHNode* root;

	/**
	 * @brief Recursively check for collision.
	 */
	bool CheckCollisions(BVHNode* node, const BoxCollider* collider) const;

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
	 * @brief Recursively delete all nodes of the tree.
	 */
	void Destroy(BVHNode* node);

public:
	/**
	 * @brief Recursively build BVH tree via top-down method.
	 */
	BVHNode* BuildTree(std::vector<BoxCollider*>& colliders);

	/**
	 * @brief Empty the tree.
	 */
	void Destroy();

	/**
	 * @brief Check for collisions.
	 */
	bool CheckCollisions(const BoxCollider* boxCollider) const;
};

#endif // !_BVH_H_
