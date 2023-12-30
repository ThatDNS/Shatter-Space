// @file: Vector2.h
//
// @brief: Header file for Vector2 class.
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-04

#pragma once
#ifndef _VECTOR_2_
#define _VECTOR_2_

class Vector2
{
public:
	float x = 0;
	float y = 0;

	Vector2() = default;
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	float Magnitude() const;
	void Normalize();

	Vector2 operator+(const Vector2& other) const
	{
		Vector2 result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}

	Vector2& operator+=(const Vector2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vector2 operator-(const Vector2& other) const
	{
		Vector2 result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}

	Vector2& operator-=(const Vector2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Vector2 operator*(float other) const
	{
		Vector2 result;
		result.x = this->x * other;
		result.y = this->y * other;
		return result;
	}
};

#endif // !_VECTOR_2_
