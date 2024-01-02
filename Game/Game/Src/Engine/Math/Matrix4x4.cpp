// @file: Matrix4x4.cpp
//
// @brief: Cpp file for Matrix4x4 class.

#include "stdafx.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3.h"

Vector3 Matrix4x4::operator*(const Vector3& other) const
{
	Vector3 result;
	result.x = other.x * data[0][0] + other.y * data[1][0] + other.z * data[2][0] + other.w * data[3][0];
	result.y = other.x * data[0][1] + other.y * data[1][1] + other.z * data[2][1] + other.w * data[3][1];
	result.z = other.x * data[0][2] + other.y * data[1][2] + other.z * data[2][2] + other.w * data[3][2];
	result.w = other.x * data[0][3] + other.y * data[1][3] + other.z * data[2][3] + other.w * data[3][3];
	return result;
}

Matrix4x4 Matrix4x4::CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
{
	float fieldOfViewInRad = 1.0f / tanf(fieldOfView * 0.5f / 180.0f * MATH_PI);

	Matrix4x4 projection;
	projection[0][0] = aspectRatio * fieldOfViewInRad;
	projection[1][1] = fieldOfViewInRad;
	projection[2][2] = farPlaneDistance / (farPlaneDistance - nearPlaneDistance);
	projection[3][2] = (-farPlaneDistance * nearPlaneDistance) / (farPlaneDistance - nearPlaneDistance);
	projection[2][3] = 1.0f;
	projection[3][3] = 0.0f;

	return projection;
}

Matrix4x4 Matrix4x4::CreateTranslation(float x, float y, float z)
{
	Matrix4x4 translation = Identity();
	translation.data[3][0] = x;
	translation.data[3][1] = y;
	translation.data[3][2] = z;
	return translation;
}

Matrix4x4 Matrix4x4::CreateRotationX(float theta)
{
	Matrix4x4 rotation;
	rotation[0][0] = 1;
	rotation[1][1] = cosf(theta);
	rotation[1][2] = sinf(theta);
	rotation[2][1] = -sinf(theta);
	rotation[2][2] = cosf(theta);
	rotation[3][3] = 1;
	return rotation;
}


Matrix4x4 Matrix4x4::CreateRotationY(float theta)
{
	Matrix4x4 rotation;
	rotation[0][0] = cosf(theta);
	rotation[0][2] = sinf(theta);
	rotation[2][0] = -sinf(theta);
	rotation[1][1] = 1.0f;
	rotation[2][2] = cosf(theta);
	rotation[3][3] = 1.0f;
	return rotation;
}

Matrix4x4 Matrix4x4::CreateRotationZ(float theta)
{
	Matrix4x4 rotation;
	rotation[0][0] = cosf(theta);
	rotation[0][1] = sinf(theta);
	rotation[1][0] = -sinf(theta);
	rotation[1][1] = cosf(theta);
	rotation[2][2] = 1;
	rotation[3][3] = 1;
	return rotation;
}

Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 identity;
	for (size_t i = 0; i < 4; ++i)
		identity[i][i] = 1.0f;
	return identity;
}

std::string Matrix4x4::ToString()
{
	std::string str_rep = "Matrix4x4(";
	for (size_t i = 0; i < 4; ++i)
		str_rep += (" {" + std::to_string(data[i][0]) + ", " + std::to_string(data[i][1]) + ", " + std::to_string(data[i][2]) + ", " + std::to_string(data[i][3]) + " },");
	str_rep += " )";
	return str_rep;
}
