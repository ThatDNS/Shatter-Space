// @file: CollisionSystem.h
//
// @brief: Header file for CollisionSystem, a singleton responsible for checking collisions.

#pragma once
#ifndef _COLLISION_SYSTEM_H_
#define _COLLISION_SYSTEM_H_

#include "Engine/Components/Collider.h"

class Vector3;
class Entity;
class BVH;

class CollisionSystem
{
	DECLARE_SINGLETON(CollisionSystem)

	// After MAX_TREE_UPDATE_ITERS updates to the BVH tree, we create a new tree
	// With each update, BVH tree can become less efficient.
	// Periodically re-creating the tree ensures efficiency.
	short int MAX_TREE_UPDATE_ITERS = 100;
	short int treeUpdateCount = 0;

	std::list<Collider*> colliders;
	// BVH for box colliders
	BVH* bvhTree = nullptr;

	void BuildNewBVHTree();

public:
	/**
	 * @brief Check if any object collided with the input collider.
	 * 
	 * @param collider Collider of the entity
	 * @param colliderTag Check collision with objects having this tag. If GENERIC, all collisions are checked.
	 * 
	 * @return Collider pointer if there was a collision. Else, nullptr.
	 */
	Collider* CheckCollision(Collider* collider, ColliderTag colliderTag = GENERIC);

	/**
	 * @brief Get normal vector to the collision plane.
	 *
	 * @param collider Collider of the entity
	 * @param checkTag Check collider tag
	 * @param colliderTag Check collision with objects having this tag.If GENERIC, all collisions are checked.
	 *
	 * @return Normal vector
	 */
	Vector3 GetCollisionNormal(Collider* collider, ColliderTag colliderTag = GENERIC);

protected:
	void AddCollider(Collider*);
	void RemoveCollider(Collider*);

	void Initialize();
	void Update();
	void Destroy();

	friend class Engine;
	friend class EntityPool;
};

#endif // !_COLLISION_SYSTEM_H_
