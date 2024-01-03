#pragma once
#ifndef _ENGINE_MATH_

class Vector3;
class Triangle;

/**
 * @brief Get the intersection point of a line and a plane
 *
 * @param planePt A point on the plane.
 * @param planeNormal Normal to the plane.
 * @param linePt1 A point on the line.
 * @param linePt2 A point on the line.
 * @return Vector3 intersection point.
 */
Vector3 GetPlaneLineIntersection(Vector3& planePt, Vector3& planeNormal, Vector3& linePt1, Vector3& linePt2);


/**
 * @brief Compute the number of triangles returned after clipping a triangle.
 * Example: Clipping a triangle can cause formation of a quad, which can be represented as 2 triangles.
 *
 * Source: YT video of OneLoneCoder
 * 
 * @param planePt A point on the plane.
 * @param planeNormal Normal to the plane.
 * @param inTri Input triangle
 * @param outTri1 1st triangle after clipping (if exists)
 * @param outTri2 2nd triangle after clipping (if exists)
 * @return int Number of triangles after clipping (0, 1, or 2)
 */
int ClipTriangleByPlane(Vector3& planePt, Vector3& planeNormal, Triangle& inTri, Triangle& outTri1, Triangle& outTri2);

#endif // !_ENGINE_MATH_
