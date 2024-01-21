// @file: Breakable.h
//
// @brief: Header file for breakable component functionalities.

#pragma once
#ifndef _BREAKABLE_H_
#define _BREAKABLE_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Mesh.h"

class MeshRenderer;
class RigidBody;
class Particles;
class UIManager;

enum BreakableType
{
	Pyramid,
	Plane,
	Star
};

class Breakable : public Component
{
	BreakableType breakableType = Pyramid;
	const int PYRAMID_SCORE = 2;
	const int PLANE_SCORE = 0;
	const int STAR_SCORE = 10;
	int _score = 0;

	// Cached components
	MeshRenderer* meshRenderer = nullptr;
	RigidBody* rigidBody = nullptr;
	Particles* particles = nullptr;
	UIManager* uiManager = nullptr;

	// Broken mesh parts
	std::list<Mesh> brokenPieces;

	// Delay in deleting the object
	bool timeToDie = false;
	float timeLeft = 1.0f;

	void LoadMeshes(std::string& meshObjFile, size_t meshPieces);
	void SpawnBrokenPieces(Mesh& mesh);

public:
	Breakable() { type = BreakableC; }
	
	void Break();
	void SetBreakableType(BreakableType bt) { breakableType = bt; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_BREAKABLE_H_
