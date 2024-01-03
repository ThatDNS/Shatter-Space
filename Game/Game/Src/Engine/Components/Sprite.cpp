// @file: Sprite.cpp
//
// @brief: Cpp file for Sprite class, responsible for loading and rendering a sprite.

#include "stdafx.h"

#include "App/app.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Sprite.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Math/Vector3.h"

IMPLEMENT_DYNAMIC_CLASS(Sprite)

void Sprite::Load(json::JSON& spriteJSON)
{
	if (spriteJSON.hasKey("Header"))
	{
		Object::Load(spriteJSON["Header"]);
	}

	// Load up sprite image data
	THROW_RUNTIME_ERROR(!spriteJSON.hasKey("Image"), "Sprite data not found.")
	THROW_RUNTIME_ERROR(!spriteJSON["Image"].hasKey("Filepath"), "Sprite file path missing.")
	THROW_RUNTIME_ERROR(!spriteJSON["Image"].hasKey("Columns"), "Sprite columns missing.")
	THROW_RUNTIME_ERROR(!spriteJSON["Image"].hasKey("Rows"), "Sprite rows missing.")
	
	json::JSON imgJSON = spriteJSON["Image"];
	std::string imgLocation = imgJSON["Filepath"].ToString();
	int imgColumns = imgJSON["Columns"].ToInt();
	int imgRows = imgJSON["Rows"].ToInt();

	csprite = App::CreateSprite(imgLocation.c_str(), imgColumns, imgRows);
}

void Sprite::Initialize()
{
	// Get transform & cache it
	transform = GetEntity()->GetTransform();

	// Set initial transform & create animations
	UpdatePosition();
	float speed = 1.0f / 15.0f;
	csprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	csprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	csprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	csprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });

	Logger::Get().Log("Initialized sprite");
}

void Sprite::Update(float deltaTime)
{
	UpdatePosition();

	// Update animation
	csprite->SetAnimation(currentAnimation);

	// Update CSimpleSprite animation
	csprite->Update(deltaTime);
}

void Sprite::UpdatePosition()
{
	// Update transform in CSimpleSprite
	csprite->SetPosition(transform->position.x, transform->position.y);
	csprite->SetAngle(transform->rotation.z);  // 2D rotation
	csprite->SetScale(transform->scale.x);
}

void Sprite::Render()
{
	csprite->Draw();

	//Logger::Get().Log("Rendered sprite");
}

void Sprite::Destroy()
{
	delete csprite;

	Logger::Get().Log("Destroyed sprite");
}
