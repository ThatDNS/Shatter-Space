// @file: Vector2.cpp
//
// @brief: Cpp file for Vector2 class.
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-04

#include "stdafx.h"
#include "Engine/Math/Vector2.h"

float Vector2::Magnitude() const
{
	return std::sqrt(x * x + y * y);
}

void Vector2::Normalize()
{
	float magnitude = Magnitude();
	if (magnitude == 0)
		return;
	x = x / magnitude;
	y = y / magnitude;
}
