// @file: DoorOpener.h
//
// @brief: Header file for DoorOpener class responsible for opening the obstacle door.

#include "stdafx.h"
#include "Game/DoorOpener.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/MeshRenderer.h"

void DoorOpener::Update(float deltaTime)
{
	if (openDoorNow)
	{
		int sign = (openLeft) ? -1 : 1;
		GetEntity()->GetTransform().Translate(Vector3(moveSpeed * sign * (deltaTime / 1000.0f), 0.0f, 0.0f));
	}
}

void DoorOpener::SetOpenDoor(bool value)
{
	openDoorNow = value;
	
	// Set mesh color appropriately
	MeshRenderer* mr = static_cast<MeshRenderer*>(GetEntity()->GetComponent(MeshRendererC));
	if (value)
		mr->SetMeshColor(Vector3(0.0f, 1.0f, 0.0f));
	else
		mr->SetMeshColor(Vector3(1.0f, 0.0f, 0.0f));
}
