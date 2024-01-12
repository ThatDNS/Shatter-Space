// @file: Renderable.cpp
//
// @brief: Cpp file for Renderable class.

#include "stdafx.h"

#include "Engine/Components/Renderable.h"
#include "Engine/Systems/RenderSystem.h"

Renderable::Renderable()
{
	RenderSystem::Get().AddRenderable(this);
}

Renderable::~Renderable()
{
	RenderSystem::Get().RemoveRenderable(this);
}
