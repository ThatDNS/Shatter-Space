// @file: BoxCollider.h
//
// @brief: Header file for BoxCollider class. Checks for box collision.

#pragma once
#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

#include "Engine/Components/Collider.h"
#include "Engine/Math/Vector3.h"

class MeshRenderer;

/**
 * @class BoxCollider
 *
 * Box collider has been implemented with the concept of AABB (axis aligned bounding box)
 */
class BoxCollider : public Collider
{
private:
	// For representing the box collider
	Vector3 minCoords;
	Vector3 maxCoords;

	// Caching mesh renderer of the entity
	MeshRenderer* meshR = nullptr;

	/**
	 * @brief Construct the box collider using the mesh renderer of this entity
	 */
	void Callibrate();

public:
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
	
	Vector3& GetMinCoords() { return minCoords; }
	Vector3& GetMaxCoords() { return maxCoords; }
};

#endif // !_BOX_COLLIDER_H_
