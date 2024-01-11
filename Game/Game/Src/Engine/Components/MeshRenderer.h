// @file: MeshRenderer.h
//
// @brief: Header file for MeshRenderer class.
// Responsible for rendering a mesh.

#pragma once
#ifndef _MESH_RENERER_H_
#define _MESH_RENERER_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Math/Mesh.h"

class Matrix4x4;

class MeshRenderer : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(MeshRenderer, Renderable)
	
	Mesh mesh;
	bool renderBackSide = false;

	Matrix4x4 GetWorldMatrix();

protected:
	MeshRenderer() = default;
	~MeshRenderer() = default;

	void Load(json::JSON&) override;
	void Initialize() override;
	void Update(float) override {}
	void Render() override;
	void Destroy() override {}

public:
	void LoadMesh(const std::string&);
	void SetRenderBackSide(bool value) { renderBackSide = value; }

	// RenderSystem is going to call Render()
	friend class RenderSystem;
	// MeshRenderer is part of an Entity
	friend class Entity;
};

#endif // !_MESH_RENERER_H_
