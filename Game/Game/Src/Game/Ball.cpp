// @file: Ball.cpp
//
// @brief: Cpp file for ball functionalities.

#include "stdafx.h"
#include "Ball.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

void Ball::Initialize()
{
	// Mark the collider as Ball collider
	BoxCollider* boxC = static_cast<BoxCollider*>(GetEntity()->GetComponent(BoxColliderC));
	boxC->SetColliderTag(BALL);
}
