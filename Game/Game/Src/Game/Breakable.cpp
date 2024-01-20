// @file: Breakable.cpp
//
// @brief: Cpp file for breakable component functionalities.

#include "stdafx.h"
#include "Breakable.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CollisionSystem.h"

void Breakable::Initialize()
{
	// Mark the collider as Ball collider
	boxC = static_cast<BoxCollider*>(GetEntity()->GetComponent(BoxColliderC));
	boxC->SetColliderTag(BREAKABLE);

	// On collision enter callback
	boxC->SetOnCollisionEnterCallback([this](Collider* collider) {
		// Die
		if (collider->GetColliderTag() == BALL)
			SceneManager::Get().GetActiveScene()->RemoveEntity(GetEntity());
	});
}

void Breakable::Update(float deltaTime)
{
}
