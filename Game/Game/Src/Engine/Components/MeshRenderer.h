// @file: MeshRenderer.h
//
// @brief: Header file for MeshRenderer class.
// Responsible for rendering a mesh.

#pragma once
#ifndef _MESH_RENERER_H_
#define _MESH_RENERER_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Math/Mesh.h"

class Matrix4x4;

class MeshRenderer : public Renderable
{	
	Mesh mesh;
	bool renderBackSide = false;

protected:
	// Protected destructor so that only Entity can delete it
	~MeshRenderer() = default;

	void Initialize() override;
	void Update(float) override { }
	void Render() override;
	void Destroy() override { }

public:
	MeshRenderer() { type = MeshRendererC; }

	void LoadMesh(const std::string&);
	Matrix4x4 GetWorldMatrix();
	const Mesh& GetMesh() { return mesh; }
	void SetRenderBackSide(bool value) { renderBackSide = value; }

	// RenderSystem is going to call Render()
	friend class RenderSystem;
	// MeshRenderer is part of an Entity
	friend class Entity;
};

#endif // !_MESH_RENERER_H_
