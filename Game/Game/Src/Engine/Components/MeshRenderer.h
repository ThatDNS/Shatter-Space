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
#include "Engine/Math/Vector3.h"

class Matrix4x4;

class MeshRenderer : public Renderable
{	
	Mesh mesh;
	bool renderBackSide = false;
	bool hideMesh = false;
	Vector3 meshColor{ 1.0f, 1.0f, 1.0f };

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
	void LoadMesh(Mesh&);
	Matrix4x4 GetWorldMatrix();

	const Mesh& GetMesh() { return mesh; }
	void SetHideMesh(bool value) { hideMesh = value; }
	void SetRenderBackSide(bool value) { renderBackSide = value; }
	void SetMeshColor(Vector3& color) { meshColor = color; }
	Vector3 GetMeshColor() const { return meshColor; }
};

#endif // !_MESH_RENERER_H_
