#include "stdafx.h"
#include "Engine/Math/EngineMath.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Triangle.h"

Vector3 GetPlaneLineIntersection(Vector3& planePt, Vector3& planeNormal, Vector3& linePt1, Vector3& linePt2)
{
	Vector3 planeN = planeNormal;
	planeN.Normalize();
	float plane_d = -Vector3::Dot(planeN, planePt);
	float ad = Vector3::Dot(linePt1, planeN);
	float bd = Vector3::Dot(linePt2, planeN);
	float t = (-plane_d - ad) / (bd - ad);
	Vector3 planeLine = linePt2 - linePt1;
	Vector3 lineToIntersect = planeLine * t;
	return (linePt1 + lineToIntersect);
}

int ClipTriangleByPlane(Vector3& planePt, Vector3& planeNormal, Triangle& inTri, Triangle& outTri1, Triangle& outTri2)
{
	// Normalize plane normal
	Vector3 planeN = planeNormal;
	planeN.Normalize();

	// Return signed shortest distance from point to plane, plane normal must be normalised
	auto dist = [&](Vector3& p)
		{
			Vector3 n = p;
			n.Normalize();
			return (Vector3::Dot(planeN, p) - Vector3::Dot(planeN, planePt));
		};

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	Vector3* inside_points[3];  int nInsidePointCount = 0;
	Vector3* outside_points[3]; int nOutsidePointCount = 0;

	// Get signed distance of each point in triangle to plane
	float d0 = dist(inTri.points[0]);
	float d1 = dist(inTri.points[1]);
	float d2 = dist(inTri.points[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &inTri.points[0]; }
	else { outside_points[nOutsidePointCount++] = &inTri.points[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &inTri.points[1]; }
	else { outside_points[nOutsidePointCount++] = &inTri.points[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &inTri.points[2]; }
	else { outside_points[nOutsidePointCount++] = &inTri.points[2]; }

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		outTri1 = inTri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// The inside point is valid, so keep that...
		outTri1.points[0] = *inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		outTri1.points[1] = GetPlaneLineIntersection(planePt, planeN, *inside_points[0], *outside_points[0]);
		outTri1.points[2] = GetPlaneLineIntersection(planePt, planeN, *inside_points[0], *outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		outTri1.points[0] = *inside_points[0];
		outTri1.points[1] = *inside_points[1];
		outTri1.points[2] = GetPlaneLineIntersection(planePt, planeN, *inside_points[0], *outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		outTri2.points[0] = *inside_points[1];
		outTri2.points[1] = outTri1.points[2];
		outTri2.points[2] = GetPlaneLineIntersection(planePt, planeN, *inside_points[1], *outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}

	return 0;
}
