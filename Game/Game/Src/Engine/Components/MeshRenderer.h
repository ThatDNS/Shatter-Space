// @file: MeshRenderer.h
//
// @brief: Header file for MeshRenderer class.
// Responsible for rendering a mesh.

#pragma once
#ifndef _MESH_RENERER_H_
#define _MESH_RENERER_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Components/Transform.h"

class MeshRenderer : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(MeshRenderer, Renderable)
	
	Mesh mesh;

	// Cached transform
	Transform* transform;

protected:
	MeshRenderer() = default;
	~MeshRenderer() = default;

	void Load(json::JSON&) override;
	void Initialize() override;
	void Update(float) override {}
	void Render() override;
	void Destroy() override {}

	// RenderSystem is going to call Render()
	friend class RenderSystem;
	// MeshRenderer is part of an Entity
	friend class Entity;
};

#endif // !_MESH_RENERER_H_
