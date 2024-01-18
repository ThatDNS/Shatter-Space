// @file: TestAABB.cpp
//
// @brief: Cpp file for TestAABB class containing unit tests for AABB class.

#include "stdafx.h"
#include "TestAABB.h"
#include "Engine/Algorithms/AABB.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Core/Logger.h"

void TestAABB::RunTests()
{
	TestIntersects();
	TestToString();
	Logger::Get().Log("[UNITTEST] AABB - All tests passed!");
}

void TestAABB::TestIntersects()
{
	AABB aabb1{ Vector3(0.0f, 0.0f, 0.0f), Vector3(5.0f, 5.0f, 5.0f) };
	AABB aabb2{ Vector3(6.0f, 7.0f, 8.0f), Vector3(10.0f, 10.0f, 10.0f) };
	AABB aabb3{ Vector3(4.0f, 4.0f, 5.0f), Vector3(7.0f, 8.0f, 9.0f) };

	assert(!aabb1.Intersects(aabb2));
	assert(aabb1.Intersects(aabb3));
}

void TestAABB::TestToString()
{
	AABB aabb1{ Vector3(1.0f, 2.0f, -1.0f), Vector3(5.0f, 5.0f, 5.0f) };
	assert(aabb1.ToString() == "AABB( min=Vector3(x=1.000000, y=2.000000, z=-1.000000), max=Vector3(x=5.000000, y=5.000000, z=5.000000) )");
}
