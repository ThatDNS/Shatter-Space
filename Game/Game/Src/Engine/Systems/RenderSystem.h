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
	const std::string DATA_FILE = "Assets/RenderSystem.json";

	std::list<Renderable*> renderables;
	std::list<Renderable*> renderablesToBeRemoved;
	std::map<STRCODE, Renderable*> uidToRenderable;

	Renderable* GetRenderable(STRCODE);

	// Camera
	Vector3 cameraPosition;
	Vector3 cameraTarget;

	// Directional light (sun)
	Vector3 lightDirection{ 0.0f, 0.0f, -1.0f };
	bool enableSun = true;

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

public:
	void AddRenderable(Renderable*);
	void RemoveRenderable(Renderable*);
	void TrackRenderable(Renderable*);
	void UntrackRenderable(Renderable*);

	Vector3& GetCameraPosition() { return cameraPosition; }
	Matrix4x4& GetViewMatrix() { return viewMatrix; }
	Matrix4x4& GetProjectionMatrix() { return projectionMatrix; }
	bool HasSun() const { return enableSun; }
	Vector3& GetSunlightDirection() { return lightDirection; }
	void SetSun(bool enable) { enableSun = enable; }
	void SetSunlight(Vector3& direction) { lightDirection = direction; }
	void SetRenderableActive(STRCODE, bool);

protected:
	void Initialize();
	void Render();

	friend class Engine;
};

#endif