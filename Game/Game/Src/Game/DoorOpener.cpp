// @file: DoorOpener.h
//
// @brief: Header file for DoorOpener class responsible for opening the obstacle door.

#include "stdafx.h"
#include "App/app.h"
#include "Game/DoorOpener.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/RenderSystem.h"
#include "Game/UIManager.h"

void DoorOpener::Initialize()
{
	// Find the UIManager
	std::list<Entity*> match = SceneManager::Get().GetActiveScene()->FindEntityWithComponent(UIManagerC);
	if (match.size() == 0)
		Logger::Get().Log("Door opener could not find UI Manager", ERROR_LOG);
	else
		uiManager = static_cast<UIManager*>(match.front()->GetComponent(UIManagerC));

	tookDamage = false;
	soundPlayed = false;
}

void DoorOpener::Update(float deltaTime)
{
	if (tookDamage)
		return;

	if (openDoorNow)
	{
		if (!soundPlayed)
		{
			App::PlaySound("Assets/Sounds/door_open.wav");
			soundPlayed = true;
		}
		int sign = (openLeft) ? -1 : 1;
		GetEntity()->GetTransform().Translate(Vector3(moveSpeed * sign * (deltaTime / 1000.0f), 0.0f, 0.0f));
	}
	// Instead of applying half damage from left & half damage from right gate,
	// we'll apply total damage from left gate and ignore the right gate.
	else if(openLeft && uiManager)
	{
		// Damage player if they get too close
		Vector3& cameraPos = RenderSystem::Get().GetCameraPosition();
		Vector3& position = GetEntity()->GetTransform().position;
		if (std::abs(position.z - std::abs(cameraPos.z)) < 2.0f)
		{
			UIBuffer damage;
			damage.position.x = APP_VIRTUAL_WIDTH / 2 - 70;
			damage.position.y = APP_VIRTUAL_HEIGHT - 60;
			damage.project = false;
			damage.timeRemaining = 2.0f;
			damage.text = "Took Damage! (-10)";
			damage.color = Vector3(1.0f, 0.0f, 0.0f);
			uiManager->ScheduleRender(damage);
			uiManager->DecreaseBalls(10);
			tookDamage = true;
		}
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
