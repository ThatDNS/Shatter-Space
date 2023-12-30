// @file: RenderSystem.h
//
// @brief: Header file for RenderSystem, a singleton responsible for handling rendering of all renderables.

#pragma once

#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

class Renderable;

class RenderSystem
{
	DECLARE_SINGLETON(RenderSystem)

private:
	std::list<Renderable*> renderables;
	std::list<Renderable*> renderablesToBeRemoved;
	std::map<STRCODE, Renderable*> uidToRenderable;

	Renderable* GetRenderable(STRCODE);

public:
	void AddRenderable(Renderable*);
	void RemoveRenderable(Renderable*);
	void TrackRenderable(Renderable*);
	void UntrackRenderable(Renderable*);

	void SetRenderableActive(STRCODE, bool);

protected:
	void Initialize();
	void Render();

	friend class Engine;
};

#endif