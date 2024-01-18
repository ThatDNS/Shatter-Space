// @file: Matrix4x4.h
//
// @brief: Header file for Matrix4x4 class.

#pragma once
#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_

class Vector3;

/**
 * @class Matrix4x4
 *
 * Class representing a 4x4 Matrix.
 */
class Matrix4x4
{
	float data[4][4] = { 0 };

public:
	// View Matrix
	static Matrix4x4 CreateLookAt(Vector3& position, Vector3& target, Vector3& up);

	// Projection Matrix (World space to Screen space)
	static Matrix4x4 CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);

	// Scale
	static Matrix4x4 CreateScale(Vector3& vScale);
	static Matrix4x4 CreateScale(float x, float y, float z);

	// Translation
	static Matrix4x4 CreateTranslation(Vector3& vPosition);
	static Matrix4x4 CreateTranslation(float x, float y, float z);

	// Rotation
	static Matrix4x4 CreateRotation(Vector3& vRotation);
	static Matrix4x4 CreateRotationX(float theta);
	static Matrix4x4 CreateRotationY(float theta);
	static Matrix4x4 CreateRotationZ(float theta);

	// Static member to create identity matrix
	static Matrix4x4 Identity();

	// Access elements
	float* operator[](size_t i) { return data[i]; }
	const float* operator[](size_t i) const { return data[i]; }

	std::string ToString();

	// -------------------------- Operator overloads --------------------------
	Matrix4x4 operator*(const Matrix4x4& other) const
	{
		Matrix4x4 result;
		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				float sum = 0.0f;

				for (size_t k = 0; k < 4; ++k)
					sum += data[i][k] * other[k][j];

				result[i][j] = sum;
			}
		}

		return result;
	}

	// Last column of 4x4 matrix is used for weighted division
	Vector3 operator*(const Vector3& other) const;

	bool operator==(const Matrix4x4& other)
	{
		bool value = true;
		for (size_t i = 0; i < 4; i++)
			for (size_t j = 0; j < 4; j++)
				value = value && (data[i][j] == other[i][j]);
		return value;
	}
};

#endif // !_MATRIX4X4_H_
