// @file: Collider.h
//
// @brief: Header file for Collider class. Creating this component causes
// the entity to be tracked by collision system.

#pragma once
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Engine/Components/Renderable.h"

enum ColliderTag {
	GENERIC,  // collides with all
	PLAYER,
	ENEMY,
	WALL,
	DOOR
};

enum ColliderType {
	BOX,
	SPHERE
};

class Vector3;

class Collider : public Renderable
{
	virtual void Callibrate() = 0;

protected:
	ColliderTag colliderTag = GENERIC;
	bool shouldRender = false;

	// A flag for collision system to check if the collider got updated by any component
	bool gotUpdated = false;

public:
	Collider() = default;
	~Collider() = default;

	virtual ColliderType GetColliderType() const = 0;

	/**
	 * @brief Check if a collider collided with this box collider.
	 */
	virtual bool DidCollide(Collider*) = 0;

	void SetColliderTag(ColliderTag tag) { colliderTag = tag; }
	ColliderTag GetColliderTag() const { return colliderTag; }
	void SetShouldRender(bool value) { shouldRender = value; }
	bool ShouldRender() const { return shouldRender; }

	void Update(float) override { }
	void Destroy() override { }

	friend class CollisionSystem;
	friend class Entity;
};

#endif // !_COLLIDER_H_
