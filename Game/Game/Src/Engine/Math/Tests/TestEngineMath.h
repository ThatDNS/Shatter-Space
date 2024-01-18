#pragma once

#include "Engine/Math/EngineMath.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Triangle.h"

void TestGetPlaneLineIntersection()
{
	Vector3 intersectionPt = GetPlaneLineIntersection(Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 2.0f, 3.0f), Vector3(-1.0f, 5.0f, -10.0f));
	assert(intersectionPt == Vector3(0.538461566f, 2.69230771f, 0.0f));
}

void TestClipTriangleByPlane()
{
	Vector3 planePt{ 0.0f, 0.0f, 0.1f };
	Vector3 planeNormal{ 0.0f, 0.0f, 1.0f };
	Triangle buffer[2];

	Triangle tri0{ Vector3(-0.5f, -1.0f, 0.086f), Vector3(-0.38f, -1.0f, -0.005f), Vector3(-0.2f, -1.0f, -0.06f) };
	Triangle tri1{ Vector3(-8.8f, 8.0f, 25.0f), Vector3(-9.2f, 8.0f, 15.0f), Vector3(-9.2f, 8.0f, 25.0f) };
	Triangle tri2{ Vector3(-0.195f, -1.0f, 0.1f), Vector3(0.0f, 1.0f, 1.09f), Vector3(0.0f, -1.0f, 0.092f) };
	
	assert(ClipTriangleByPlane(planePt, planeNormal, tri0, buffer[0], buffer[1]) == 0);
	assert(ClipTriangleByPlane(planePt, planeNormal, tri1, buffer[0], buffer[1]) == 1);
	assert(ClipTriangleByPlane(planePt, planeNormal, tri2, buffer[0], buffer[1]) == 2);
}
