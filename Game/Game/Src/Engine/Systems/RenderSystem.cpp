// @file: RenderSystem.cpp
//
// @brief: Cpp file for RenderSystem, a singleton responsible for handling rendering of all renderables.

#include "stdafx.h"
#include "app/app.h"

#include "Engine/Systems/RenderSystem.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Renderable.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"

void RenderSystem::AddRenderable(Renderable* renderable)
{
	// Pushing in front so that the entities added later get rendered first
	// This ensures that older entities are in front
	renderables.push_front(renderable);
	uidToRenderable[renderable->GetUid()] = renderable;
}

void RenderSystem::RemoveRenderable(Renderable* renderable)
{
	uidToRenderable.erase(renderable->GetUid());
	renderables.remove(renderable);
}

Renderable* RenderSystem::GetRenderable(STRCODE id)
{
	auto itr = uidToRenderable.find(id);
	if (itr == uidToRenderable.end())
	{
		std::string msg = "Error in getting " + std::to_string(id) + " renderable";
		Logger::Get().Log(msg, ERROR_LOG);
		return nullptr;
	}
	return uidToRenderable[id];
}

void RenderSystem::SetRenderableActive(STRCODE renderableID, bool _active)
{
	Renderable* renderable = GetRenderable(renderableID);
	if (renderable)
		renderable->SetActive(_active);
}

void RenderSystem::Initialize()
{
	// Populate uidToRenderable map here. Can't do it in constructor because UID of renderables get changed on Component::Load
	for (Renderable* renderable : renderables)
	{
		uidToRenderable[renderable->GetUid()] = renderable;
	}
	
	// Set initial camera position and target
	cameraPosition = { 0.0f, 0.0f, 0.0f };
	cameraLookDir = { 0.0f, 0.0f, 1.0f };

	projectionMatrix = Matrix4x4::CreatePerspectiveFieldOfView(60.0f, (float)APP_INIT_WINDOW_HEIGHT / (float)APP_INIT_WINDOW_WIDTH, 1.0f, 500.0f);
}

void RenderSystem::Update(float deltaTime)
{
	if (toFollow)
	{
		const Vector3& tPos = toFollow->GetTransform().position;
		cameraPosition = -tPos;
		//Vector3::Lerp(cameraPosition, -tPos, cameraSpeed * deltaTime / 100.0f);
	}
}

void RenderSystem::Render()
{
	cameraTarget = cameraPosition + cameraLookDir;

	// Update view matrix (to be used in renderables)
	viewMatrix = Matrix4x4::CreateLookAt(cameraPosition, cameraTarget, Vector3(0.0f, 1.0f, 0.0f));

	for (Renderable* renderable : renderables)
	{
		if (renderable->IsActive())
		{
			renderable->Render();
		}
	}
}
