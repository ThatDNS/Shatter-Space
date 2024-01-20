// @file: Ball.cpp
//
// @brief: Cpp file for ball functionalities.

#include "stdafx.h"
#include "Ball.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

void Ball::Initialize()
{
}

void Ball::Update(float deltaTime)
{
	if (GetEntity()->GetTransform().position.y <= minYPos)
	{
		// Remove this entity from the scene
		SceneManager::Get().GetActiveScene()->RemoveEntity(GetEntity());
	}
}
