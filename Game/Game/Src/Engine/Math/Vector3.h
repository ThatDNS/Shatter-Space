// @file: Vector3.h
//
// @brief: Header file for Vector3 class.

#pragma once
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;  // Used in 4x4 matrix multiplication

	Vector3() = default;
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1.0f) {}

	float Magnitude() const;
	void Normalize();
	
	float Distance(const Vector3&) const;

	static float Dot(const Vector3&, const Vector3&);
	static Vector3 Cross(const Vector3&, const Vector3&);

	// Linear Interpolation
	static void Lerp(Vector3&, const Vector3&, float);

	std::string ToString() const;

	// ---------------------- Operator overloaders ----------------------

	Vector3 operator+(const Vector3& other) const
	{
		Vector3 result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		result.z = this->z + other.z;
		return result;
	}

	Vector3 operator-(const Vector3& other) const
	{
		Vector3 result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		result.z = this->z - other.z;
		return result;
	}

	Vector3 operator*(float other) const
	{
		Vector3 result;
		result.x = this->x * other;
		result.y = this->y * other;
		result.z = this->z * other;
		return result;
	}

	Vector3 operator/(float other) const
	{
		Vector3 result;
		result.x = this->x / other;
		result.y = this->y / other;
		result.z = this->z / other;
		return result;
	}

	Vector3& operator+=(const Vector3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	Vector3& operator+=(float other)
	{
		this->x += other;
		this->y += other;
		this->z += other;
		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	Vector3& operator-=(float other)
	{
		this->x -= other;
		this->y -= other;
		this->z -= other;
		return *this;
	}

	Vector3& operator*=(float other)
	{
		this->x *= other;
		this->y *= other;
		this->z *= other;
		return *this;
	}

	Vector3& operator/=(float other)
	{
		this->x /= other;
		this->y /= other;
		this->z /= other;
		return *this;
	}

	// Reading X, Y, Z as 0, 1, 2 indices
	float operator[](int idx) const
	{
		if (idx == 0) return x;
		if (idx == 1) return y;
		if (idx == 2) return z;
		return -1;
	}

	bool operator==(const Vector3& other)
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	bool operator!=(const Vector3& other)
	{
		return (x != other.x || y != other.y || z != other.z);
	}
};

#endif // !_Vector3_H_
