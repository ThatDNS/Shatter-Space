// @file: Matrix4x4.cpp
//
// @brief: Cpp file for Matrix4x4 class.

#include "stdafx.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3.h"

Vector3 Matrix4x4::operator*(const Vector3& other) const
{
	Vector3 result;
	result.x = other.x * data[0][0] + other.y * data[1][0] + other.z * data[2][0] + data[3][0];
	result.y = other.x * data[0][1] + other.y * data[1][1] + other.z * data[2][1] + data[3][1];
	result.z = other.x * data[0][2] + other.y * data[1][2] + other.z * data[2][2] + data[3][2];

	float wt = other.x * data[0][3] + other.y * data[1][3] + other.z * data[2][3] + data[3][3];
	if (wt != 0.0f)
	{
		result.x /= wt;
		result.y /= wt;
		result.z /= wt;
	}
	return result;
}
