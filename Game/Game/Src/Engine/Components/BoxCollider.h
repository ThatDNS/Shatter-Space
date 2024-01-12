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
class BoxCollider// : public Collider
{
private:
	// For representing the box collider
	Vector3 minCoords;
	Vector3 maxCoords;

	// Caching mesh renderer of the entity
	MeshRenderer* meshR = nullptr;

public:
	//ColliderType GetColliderType() const override { return BOX; }

	void AttachMesh(MeshRenderer*);
	//void Initialize() override;

	/**
	 * @brief Construct the box collider using the mesh renderer of this entity
	 */
	void Callibrate();

	//bool DidCollide(Collider* collider) override;
	//bool WillCollide(Collider* collider, Vector3& newPos) override;

	/**
	 * @brief Function used to render the box collider.
	 * Useful for debugging.
	 */
	void Draw();

	//bool WillCollide(SphereCollider* collider, Vector3& newPos);
	//bool WillCollide(float colliderRadius, Vector3& newPos) override;
};

#endif // !_BOX_COLLIDER_H_
