// @file: TestVector3.cpp
//
// @brief: Cpp file for TestVector3 class containing unit tests for Vector3 class.

#include "stdafx.h"
#include "TestVector3.h"
#include "Engine/Math/Vector3.h"

void TestVector3::RunTests()
{
	TestConstructor();
	TestMagnitude();
	TestNormalize();
	TestReset();
	TestDistance();
	TestDotProduct();
	TestCrossProduct();
	TestLerp();
	TestToString();
	TestOperatorOverloads();
}

void TestVector3::TestConstructor()
{
	Vector3 vec1;
	assert(vec1.x == 0.0f && vec1.y == 0.0f && vec1.z == 0.0f && vec1.w == 1.0f);

	Vector3 vec2{ 1.5f, 2.3f, 9.2f };
	assert(vec2.x == 1.5f && vec2.y == 2.3f && vec2.z == 9.2f && vec2.w == 1.0f);
}

void TestVector3::TestMagnitude()
{
	Vector3 vec1;
	assert(vec1.Magnitude() == 0.0f);
	
	Vector3 vec2{ 22.5f, 20.0324f, 87.5f };
	assert(vec2.Magnitude() == 92.5407867f);
}

void TestVector3::TestNormalize()
{
	Vector3 vec1;
	vec1.Normalize();
	assert(vec1.x == 0.0f && vec1.y == 0.0f && vec1.z == 0.0f && vec1.w == 1.0f);

	Vector3 vec2{ 1.0f, 1.0f, 1.0f };
	vec2.Normalize();
	assert(vec2.x == 0.577350259f && vec2.y == 0.577350259f && vec2.z == 0.577350259f && vec2.w == 1.0f);

	Vector3 vec3{ 22.5f, 20.0324f, 87.5f };
	vec3.Normalize();
	assert(vec3.x == 0.243136033f && vec3.y == 0.216471031f && vec3.z == 0.945529044f && vec2.w == 1.0f);
}

void TestVector3::TestReset()
{
	Vector3 vec1;
	vec1.Reset();
	assert(vec1.x == 0.0f && vec1.y == 0.0f && vec1.z == 0.0f && vec1.w == 1.0f);

	Vector3 vec2{ 22.5f, 20.0324f, 87.5f };
	vec2.Reset();
	assert(vec2.x == 0.0f && vec2.y == 0.0f && vec2.z == 0.0f && vec2.w == 1.0f);
}

void TestVector3::TestDistance()
{
	Vector3 vec1{ -12.5f, 2.3f, 9.2f };
	Vector3 vec2{ 22.5f, 20.0324f, -87.5f };
	float dist = vec1.Distance(vec2);
	assert(dist == 104.356735f);

	Vector3 vec3{ -22.5f, -20.0324f, -87.5f };
	dist = vec1.Distance(vec3);
	assert(dist == 99.7478104f);
}

void TestVector3::TestDotProduct()
{
	Vector3 vec1{ 2.5f, 12.3f, 99.2f };
	Vector3 vec2{ 10.5f, -6.24f, 10.5f };
	assert(Vector3::Dot(vec1, vec2) == 991.097961f);

	Vector3 vec3{ 12.5f, 0.3f, -9.2f };
	assert(Vector3::Dot(vec2, vec3) == 32.7780075f);
}

void TestVector3::TestCrossProduct()
{
	Vector3 vec1{ 1.0f, 0.0f, 0.0f };
	Vector3 vec2{ 0.0f, 1.0f, 0.0f };
	assert(Vector3::Cross(vec1, vec2) == Vector3(0.0f, 0.0f, 1.0f));

	Vector3 vec3{ -1.5f, -1.3f, -1.2f };
	assert(Vector3::Cross(vec1, vec3) == Vector3(0.0f, 1.2f, -1.29999995f));
}

void TestVector3::TestLerp()
{
	Vector3 vec1{ -1.5f, -1.3f, -1.2f };
	Vector3::Lerp(vec1, Vector3(0.0f, 0.0f, 0.0f), 0.05f);
	assert(vec1.x == -1.42499995f && vec1.y == -1.2349999f && vec1.z == -1.1400001f);

	Vector3::Lerp(vec1, Vector3(5.0f, -10.0f, 86.0f), 1.0f);
	assert(vec1.x == 5.0f && vec1.y == -10.0f && vec1.z == 86.0f);
}

void TestVector3::TestToString()
{
	Vector3 vec1{ -1.5f, 1.3f, -1.002f };
	assert(vec1.ToString() == "Vector3(x=-1.500000, y=1.300000, z=-1.002000)");
}

void TestVector3::TestOperatorOverloads()
{
	Vector3 vec;

	vec = Vector3(1.0f, 1.2f, 3.4f) + Vector3(1.0f, 2.0f, -1.5f);
	assert(vec == Vector3(2.0f, 3.20000005f, 1.9000001f));

	vec = Vector3(1.0f, 1.2f, 3.4f) - Vector3(1.0f, 2.0f, -1.5f);
	assert(vec == Vector3(0.0f, -0.799999952f, 4.9000001f));

	vec = Vector3(1.5f, -0.5f, 1.25f) * 10.0f;
	assert(vec == Vector3(15.0f, -5.0f, 12.5f));

	vec = Vector3(1.5f, -0.5f, 1.25f) / 0.75f;
	assert(vec == Vector3(2.0f, -0.666666687f, 1.66666663f));

	vec = Vector3(0, 0, 0);

	vec += Vector3(1.5f, 3.4f, -10.0f);
	assert(vec == Vector3(1.5f, 3.4f, -10.0f));

	vec += 2.35f;
	assert(vec == Vector3(3.8499999f, 5.75f, -7.6500001f));

	vec -= Vector3(2.5f, 1.2f, 0.15f);
	assert(vec == Vector3(1.3499999f, 4.55000019f, -7.80000019f));

	vec -= 0.75f;
	assert(vec == Vector3(0.599999905f, 3.80000019f, -8.55000019f));

	vec *= 12.5f;
	assert(vec == Vector3(7.49999905f, 47.5000038f, -106.875f));
	
	vec /= 12.5f;
	assert(vec == Vector3(0.599999905f, 3.80000019f, -8.55000019f));

	vec = -vec;
	assert(vec == Vector3(-0.599999905f, -3.80000019f, 8.55000019f));

	assert(vec[0] == -0.599999905f && vec[1] ==  - 3.80000019f && vec[2] == 8.55000019f);
	assert(vec != Vector3(0.0f, 10.3f, 0.12f));
}
