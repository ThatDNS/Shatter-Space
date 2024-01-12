// @file: CollisionSystem.h
//
// @brief: Header file for CollisionSystem, a singleton responsible for checking collisions.

#pragma once
#ifndef _COLLISION_SYSTEM_H_
#define _COLLISION_SYSTEM_H_

class Collider;
class Vector3;
class Entity;
enum ColliderTag;

class CollisionSystem
{
	DECLARE_SINGLETON(CollisionSystem)

	std::list<Collider*> colliders;

public:
	void AddCollider(Collider*);
	void RemoveCollider(Collider*);

	// Check if an entity can be moved to a certain space or not
	bool IsSpaceEmpty(Entity*, Vector3&);
	// Check if an entity with given radius and at given locations is colliding with anything
	bool IsSpaceEmpty(float, Vector3&);

	// Check if an entity collided with another entity having a specific type of collider
	Collider* CheckIfCollided(Entity*, ColliderTag);
};

#endif // !_COLLISION_SYSTEM_H_
