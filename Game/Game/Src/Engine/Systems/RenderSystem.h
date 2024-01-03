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
	std::list<Renderable*> renderables;
	std::list<Renderable*> renderablesToBeRemoved;
	std::map<STRCODE, Renderable*> uidToRenderable;

	Renderable* GetRenderable(STRCODE);

	// Camera
	Vector3 cameraPosition;

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
	void SetRenderableActive(STRCODE, bool);

protected:
	void Initialize();
	void Render();

	friend class Engine;
};

#endif