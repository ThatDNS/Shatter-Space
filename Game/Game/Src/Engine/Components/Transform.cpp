// @file: Transform.cpp
//
// @brief: Cpp file for Transform class.

#include "stdafx.h"

#include "Engine/Components/Transform.h"
#include "Engine/Math/Vector3.h"

void Transform::Translate(const Vector3& delta) {
	position += delta;
}

void Transform::Rotate(const Vector3& delta) {
	rotation += delta;
}

