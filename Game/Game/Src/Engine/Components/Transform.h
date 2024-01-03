// @file: Transform.h
//
// @brief: Header file for Transform class.
// Stores position, rotation, and scale of an object

#pragma once
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"

class Transform : public Component {
    DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component)

public:
    Vector3 position{ 0.0f, 0.0f, 0.0f };
    Vector3 rotation{ 0.0f, 0.0f, 0.0f };  // degrees
    Vector3 scale{ 1.0f, 1.0f, 1.0f };

    Transform() = default;

    void Initialize() override {}
    void Update(float) override {}
    void Load(json::JSON& node) override;
    void Destroy() override {}

    void Translate(const Vector3& delta);
    void Rotate(const Vector3& delta);
};

#endif // !_TRANSFORM_H_
