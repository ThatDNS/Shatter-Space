// @file: Component.cpp
//
// @brief: Cpp file for the Component class.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-03

#include "stdafx.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Entity.h"

IMPLEMENT_ABSTRACT_CLASS(Component)

void Component::Load(json::JSON& node)
{
    Object::Load(node);
}

void Component::ChangeEntity(Entity* _entity)
{
    entity = _entity;
}
