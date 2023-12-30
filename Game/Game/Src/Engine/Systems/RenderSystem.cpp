// @file: RenderSystem.cpp
//
// @brief: Cpp file for RenderSystem, a singleton responsible for handling rendering of all renderables.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-04

#include "stdafx.h"

#include "Engine/Systems/RenderSystem.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/Renderable.h"

void RenderSystem::AddRenderable(Renderable* renderable)
{
	// Can not add to uidToRenderable at this point as the renderable's UID might not be
	// as expected. UID gets set in Load() whereas AddRenderable() gets called by constructor
	renderables.push_back(renderable);
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

void RenderSystem::TrackRenderable(Renderable* renderable)
{
	uidToRenderable[renderable->GetUid()] = renderable;
}

// Useful in object pooling
void RenderSystem::UntrackRenderable(Renderable* renderable)
{
	renderables.remove(renderable);
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
}

void RenderSystem::Render()
{
	for (Renderable* renderable : renderables)
	{
		if (renderable->IsActive())
		{
			renderable->Render();
		}
	}
}
