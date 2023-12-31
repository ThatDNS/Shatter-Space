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
	return std::sqrt(pow(other.x - x, 2) + pow(other.y - y, 2) + pow(other.z - z, 2));
}

Vector3 Vector3::Dot(const Vector3& other) const
{
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return Vector3(y * other.z - z * other.y,
		           z * other.x - x * other.z,
		           x * other.y - y * other.x);
}
