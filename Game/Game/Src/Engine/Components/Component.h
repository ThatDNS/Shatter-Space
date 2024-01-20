// @file: Component.h
//
// @brief: Header file for the Component class. Each renderable object is a Component.

#pragma once

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Engine/Core/Object.h"

class Entity;

// Any new derived class of Component must be added here (except abstract classes)
enum ComponentType {
    UNDEFINEDC,  // default value
    // Engine components
    TransformC,
    MeshRendererC,
    SpriteC,
    BoxColliderC,
    RigidBodyC,
    ParticlesC,
    CanvasC,
    // Game components
    PlayerC,
    BallSpawnerC,
    BallC,
    BreakableC,
    SelfDestructC,
    LevelGeneratorC
};

class Component : public Object
{
protected:
    Entity* entity = nullptr;
    ComponentType type = UNDEFINEDC;

public:
    virtual void Update(float) = 0;

    void ChangeEntity(Entity*);
    Entity* GetEntity() const { return entity; };

    friend class Entity;
    friend class EntityPool;
};

#endif // !_COMPONENT_H_
