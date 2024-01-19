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

	// Get normal to the intersection plane between two AABBs
	Vector3 GetIntersectionNormal(const AABB& other) const
	{
		// Need to find the plane / axis on which the two AABBs intesected most recently
		// Find the axis along which we have minimum intersection
		float xIntersect = std::min(maxCoords.x, other.maxCoords.x) - std::max(minCoords.x, other.minCoords.x);
		float yIntersect = std::min(maxCoords.y, other.maxCoords.y) - std::max(minCoords.y, other.minCoords.y);
		float zIntersect = std::min(maxCoords.z, other.maxCoords.z) - std::max(minCoords.z, other.minCoords.z);
		if (xIntersect < std::min(yIntersect, zIntersect))
			return Vector3(1.0f, 0.0f, 0.0f);
		if (yIntersect < std::min(xIntersect, zIntersect))
			return Vector3(0.0f, 1.0f, 0.0f);
		if (zIntersect < std::min(xIntersect, yIntersect))
			return Vector3(0.0f, 0.0f, 1.0f);

		// Does not intersect
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	std::string ToString() const
	{
		return "AABB( min=" + minCoords.ToString() + ", max=" + maxCoords.ToString() + " )";
	}
};

#endif // !_AABB_H_
