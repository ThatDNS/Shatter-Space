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
public:
	float data[4][4] = { 0 };

	// Static member to create identity matrix
	static Matrix4x4 Identity() {
		Matrix4x4 identity;
		for (size_t i = 0; i < 4; ++i)
			identity[i][i] = 1.0f;
		return identity;
	}

	// Access elements
	float* operator[](size_t i) { return data[i]; }
	const float* operator[](size_t i) const { return data[i]; }

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

};

#endif // !_MATRIX4X4_H_
