// @file: Collider.h
//
// @brief: Header file for Collider class. Creating this component causes
// the entity to be tracked by collision system.

#pragma once
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Core/Logger.h"

enum ColliderTag {
	GENERIC,  // collides with all
	BALL,
	BREAKABLE
};

enum ColliderType {
	BOX,
	SPHERE  // NOT IMPLEMENTED
};

class Vector3;

class Collider : public Renderable
{
	// type for collision callback function
	using OnCollisionCallback = std::function<void(Collider*)>;

	virtual void Callibrate() = 0;

protected:
	ColliderTag colliderTag = GENERIC;
	bool shouldRender = false;

	// A flag for collision system to check if the collider got updated by any component
	bool gotUpdated = false;

	// Called in OnCollisionEnter
	OnCollisionCallback OnCollisionEnterFunc = nullptr;

public:
	Collider() = default;
	~Collider() = default;

	virtual ColliderType GetColliderType() const = 0;

	/**
	 * @brief Check if a collider collided with this box collider.
	 */
	virtual bool DidCollide(Collider*) = 0;

	/**
	 * @brief Gets called by Entity::Move() when collision happens.
	 */
	void OnCollisionEnter(Collider* other);
	void SetOnCollisionEnterCallback(OnCollisionCallback callback) { OnCollisionEnterFunc = callback; }

	void SetColliderTag(ColliderTag tag) { colliderTag = tag; }
	ColliderTag GetColliderTag() const { return colliderTag; }
	void SetShouldRender(bool value) { shouldRender = value; }
	bool ShouldRender() const { return shouldRender; }

	void Update(float) override { }
	void Destroy() override { }

	friend class CollisionSystem;
	friend class PhysicsSystem;
	friend class Entity;
};

#endif // !_COLLIDER_H_
