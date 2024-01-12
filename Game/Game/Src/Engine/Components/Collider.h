// @file: Collider.h
//
// @brief: Header file for Collider class. Creating this component causes
// the entity to be tracked by collision system.

#pragma once
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Engine/Components/Component.h"

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

class Collider : public Component
{
private:
	ColliderTag colliderTag = GENERIC;

public:
	Collider();
	~Collider();

	virtual ColliderType GetColliderType() const = 0;

	// Check if a collider collided with another collider
	virtual bool DidCollide(Collider*) = 0;

	// Check if a collider will collide with another collider at the given location
	virtual bool WillCollide(Collider*, Vector3&) = 0;

	void SetColliderTag(ColliderTag tag) { colliderTag = tag; }
	ColliderTag GetColliderTag() const { return colliderTag; }

	void Update(float) override { }
	void Destroy() override { }
};

#endif // !_COLLIDER_H_
