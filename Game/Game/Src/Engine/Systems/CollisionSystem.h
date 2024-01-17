// @file: CollisionSystem.h
//
// @brief: Header file for CollisionSystem, a singleton responsible for checking collisions.

#pragma once
#ifndef _COLLISION_SYSTEM_H_
#define _COLLISION_SYSTEM_H_

class Collider;
class Vector3;
class Entity;
class BVH;
enum ColliderTag;

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
	bool CheckCollision(Collider*);

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
