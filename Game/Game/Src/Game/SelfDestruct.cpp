// @file: SelfDestruct.cpp
//
// @brief: Cpp file for SelfDestruct class which deletes an entity if it goes beyond the specified bound.

#include "stdafx.h"
#include "SelfDestruct.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

void SelfDestruct::Update(float deltaTime)
{
	// Checking only Y for now
	if (GetEntity()->GetTransform().position.y <= bound.y)
	{
		// Remove this entity from the scene
		SceneManager::Get().GetActiveScene()->RemoveEntity(GetEntity());
	}
}
