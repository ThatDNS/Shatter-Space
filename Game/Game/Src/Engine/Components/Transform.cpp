// @file: Transform.cpp
//
// @brief: Cpp file for Transform class.

#include "stdafx.h"

#include "Engine/Components/Transform.h"
#include "Engine/Math/Vector3.h"

IMPLEMENT_DYNAMIC_CLASS(Transform)

void Transform::Load(json::JSON& transformJSON)
{
	if (transformJSON.hasKey("Position")) {
		position.x = (float)transformJSON["Position"][0].ToFloat();
		position.y = (float)transformJSON["Position"][1].ToFloat();
		position.z = (float)transformJSON["Position"][2].ToFloat();
	}
	if (transformJSON.hasKey("Rotation"))
	{
		rotation.x = (float)transformJSON["Rotation"][0].ToFloat();
		rotation.y = (float)transformJSON["Rotation"][1].ToFloat();
		rotation.z = (float)transformJSON["Rotation"][2].ToFloat();
	}
	if (transformJSON.hasKey("Scale"))
	{
		scale.x = (float)transformJSON["Scale"][0].ToFloat();
		scale.y = (float)transformJSON["Scale"][1].ToFloat();
		scale.z = (float)transformJSON["Scale"][2].ToFloat();
	}
}

void Transform::Translate(const Vector3& delta) {
	position += delta;
}

void Transform::Rotate(const Vector3& delta) {
	rotation += delta;
}

