// @file: Component.h
//
// @brief: Header file for the Component class. Each renderable object is a Component.

#pragma once

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Engine/Core/Object.h"

class Entity;

class Component : public Object
{
    DECLARE_ABSTRACT_DERIVED_CLASS(Component, Object)

protected:
    Entity* entity = nullptr;

public:
    virtual void Load(json::JSON& node) override;
    virtual void Update(float) = 0;

    void ChangeEntity(Entity*);
    Entity* GetEntity() const { return entity; };

    friend class Entity;
};

#endif // !_COMPONENT_H_
