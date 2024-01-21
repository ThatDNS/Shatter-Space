// @file: SelfDestruct.cpp
//
// @brief: Cpp file for SelfDestruct class which deletes an entity if it goes beyond the specified bound.

#include "stdafx.h"
#include "SelfDestruct.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Core/Logger.h"

void SelfDestruct::Initialize()
{
	// Find the player entity and cache it
	std::list<Entity*> matchedEntities = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(BallSpawnerC);
	if (matchedEntities.size() == 0)
		Logger::Get().Log("Could not find ball spawner");
	else
		playerEntity = matchedEntities.front();
}

void SelfDestruct::Update(float deltaTime)
{
	Vector3& position = GetEntity()->GetTransform().position;

	if (position.y <= bound.y)
	{
		// Remove this entity from the scene
		SceneManager::Get().GetActiveScene()->RemoveEntity(GetEntity());
	}
	else if (playerEntity != nullptr)
	{
		// If the object goes beyond the player, self destruct
		if (playerEntity->GetTransform().position.z > position.z + bound.z)
			SceneManager::Get().GetActiveScene()->RemoveEntity(GetEntity());
	}
}
