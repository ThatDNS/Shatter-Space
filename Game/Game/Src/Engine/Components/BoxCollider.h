// @file: BoxCollider.h
//
// @brief: Header file for BoxCollider class. Checks for box collision.

#pragma once
#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

#include "Engine/Components/Collider.h"
#include "Engine/Algorithms/AABB.h"

class MeshRenderer;

/**
 * @class BoxCollider
 *
 * Box collider has been implemented with the concept of AABB (axis aligned bounding box)
 */
class BoxCollider : public Collider
{
private:
	// Caching mesh renderer of the entity
	MeshRenderer* meshR = nullptr;

	/**
	 * @brief Construct the box collider using the mesh renderer of this entity
	 */
	void Callibrate();

public:
	// The actual collider is stored as an AABB
	AABB boundingBox;

	BoxCollider() { type = BoxColliderC; }

	ColliderType GetColliderType() const override { return BOX; }

	void Initialize() override;
	void Update(float) override;

	/**
	 * @brief Draw box collider edge lines.
	 * Useful for debugging.
	 */
	void Render();

	void Destroy() override { }

	bool DidCollide(Collider* collider) override;
};

#endif // !_BOX_COLLIDER_H_
