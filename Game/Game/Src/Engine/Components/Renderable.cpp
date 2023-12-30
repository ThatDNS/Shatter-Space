// @file: Renderable.cpp
//
// @brief: Cpp file for Renderable class.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-04

#include "stdafx.h"
#include "Engine/Components/Renderable.h"
#include "Engine/Systems/RenderSystem.h"

IMPLEMENT_ABSTRACT_CLASS(Renderable)

Renderable::Renderable()
{
	RenderSystem::Get().AddRenderable(this);
}

Renderable::~Renderable()
{
	RenderSystem::Get().RemoveRenderable(this);
}
