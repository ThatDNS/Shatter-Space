// @file: AABB.h
//
// @brief: Header file for AABB (Axis Aligned Bounding Box) implementation class.

#pragma once
#ifndef _AABB_H_
#define _AABB_H_

#include "Engine/Math/Vector3.h"

class AABB
{
public:
	Vector3 minCoords;
	Vector3 maxCoords;

	AABB() = default;
	AABB(const Vector3& min, const Vector3& max) : minCoords(min), maxCoords(max) {}

	// Intersection between 2 AABBs
	bool Intersects(const AABB& other) const
	{
		return ((minCoords.x <= other.maxCoords.x && maxCoords.x >= other.minCoords.x) &&
				(minCoords.y <= other.maxCoords.y && maxCoords.y >= other.minCoords.y) &&
				(minCoords.z <= other.maxCoords.z && maxCoords.z >= other.minCoords.z));
	}
};

#endif // !_AABB_H_
