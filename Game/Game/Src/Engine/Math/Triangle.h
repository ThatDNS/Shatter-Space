// @file: Triangle.h
//
// @brief: Header file for Triangle class.

#pragma once
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Engine/Math/Vector3.h";

class Triangle
{
public:
	Vector3 points[3];

	Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3) : points{ v1, v2, v3 } { }
};

#endif // !_TRIANGLE_H_
