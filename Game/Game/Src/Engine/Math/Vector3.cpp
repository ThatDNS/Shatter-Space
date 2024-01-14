// @file: Vector3.cpp
//
// @brief: Source file for Vector3 class.

#include "stdafx.h"
#include "Engine/Math/Vector3.h"

float Vector3::Magnitude() const
{
	return std::sqrt(x * x + y * y + z * z);
}

void Vector3::Normalize()
{
	float magnitude = Magnitude();
	if (magnitude == 0)
		return;
	x = x / magnitude;
	y = y / magnitude;
	z = z / magnitude;
}

float Vector3::Distance(const Vector3& other) const
{
	return (float) std::sqrt(pow(other.x - x, 2) + pow(other.y - y, 2) + pow(other.z - z, 2));
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y,
				   v1.z * v2.x - v1.x * v2.z,
				   v1.x * v2.y - v1.y * v2.x);
}

std::string Vector3::ToString() const
{
	return "Vector3(x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z=" + std::to_string(z) + ")";
}
