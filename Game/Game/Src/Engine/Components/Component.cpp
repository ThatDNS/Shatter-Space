// @file: Component.cpp
//
// @brief: Cpp file for the Component class.

#include "stdafx.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Entity.h"

void Component::ChangeEntity(Entity* _entity)
{
    entity = _entity;
}
