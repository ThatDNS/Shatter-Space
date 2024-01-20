// @file: Breakable.h
//
// @brief: Header file for breakable component functionalities.

#pragma once
#ifndef _BREAKABLE_H_
#define _BREAKABLE_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Mesh.h"

class MeshRenderer;
class BoxCollider;
class Particles;
class UIManager;

enum BreakableType
{
	Pyramid,
	Plane
};

class Breakable : public Component
{
	BreakableType breakableType = Pyramid;

	// Cached components
	MeshRenderer* meshRenderer = nullptr;
	BoxCollider* boxC = nullptr;
	Particles* particles = nullptr;
	UIManager* uiManager = nullptr;

	// Broken mesh parts
	size_t numPieces = 0;
	std::list<Mesh> brokenPieces;

	// Delay in deleting the object
	bool timeToDie = false;
	float timeLeft = 1.0f;

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
