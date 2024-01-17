// @file: RenderSystem.h
//
// @brief: Header file for RenderSystem, a singleton responsible for handling rendering of all renderables.

#pragma once

#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4x4.h"

class Renderable;

class RenderSystem
{
	DECLARE_SINGLETON(RenderSystem)

private:
	float cameraSpeed = 0.4f;

	std::list<Renderable*> renderables;
	std::list<Renderable*> renderablesToBeRemoved;
	std::map<STRCODE, Renderable*> uidToRenderable;

	Renderable* GetRenderable(STRCODE);

	// Camera
	Vector3 cameraPosition;
	Vector3 cameraLookDir;
	Vector3 cameraTarget;
	Entity* toFollow = nullptr;

	// Directional light (sun)
	Vector3 lightDirection{ -0.5f, 0.0f, -1.0f };
	bool enableSun = true;

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

public:
	void SetRenderableActive(STRCODE, bool);

	// ------------------ Camera ------------------
	Vector3& GetCameraPosition() { return cameraPosition; }
	Matrix4x4& GetViewMatrix() { return viewMatrix; }
	Matrix4x4& GetProjectionMatrix() { return projectionMatrix; }
	// Make the camera follow an entity
	void AttachCamera(Entity* entityToFollow) { toFollow = entityToFollow; }
	void FreeCamera() { toFollow = nullptr; };
	
	// ------------------ Light source ------------------
	bool HasSun() const { return enableSun; }
	Vector3& GetSunlightDirection() { return lightDirection; }
	void SetSun(bool enable) { enableSun = enable; }
	void SetSunlight(Vector3& direction) { lightDirection = direction; }

protected:
	void AddRenderable(Renderable*);
	void RemoveRenderable(Renderable*);

	void Initialize();
	void Update(float);
	void Render();

	friend class Engine;
	friend class EntityPool;
};

#endif