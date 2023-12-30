// @file: Transform.h
//
// @brief: Header file for Transform class.
// Stores position, rotation, and scale of an object
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-04

#pragma once
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Vector2.h"

class Transform : public Component {
    DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component)

public:
    Vector2 position{};
    float rotation;
    Vector2 scale{};

    Transform();

    void Initialize() override {}
    void Update(float) override {}
    void Load(json::JSON& node) override;
    void Destroy() override;

    void Translate(const Vector2& delta);
    void Rotate(float delta);
    void Scale(const Vector2& delta);
};

#endif // !_TRANSFORM_H_
