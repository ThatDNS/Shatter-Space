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

class Breakable : public Component
{
	// Intentionally written without extension.
	// Names of breakable parts of this mesh have just indices appended to this filename.
	std::string meshObjFile = "Assets/Objects/Breakable/Pyramid";

	// Cached components
	MeshRenderer* meshRenderer = nullptr;
	BoxCollider* boxC = nullptr;
	Particles* particles = nullptr;

	// Broken mesh parts
	size_t numPieces = 4;
	std::list<Mesh> brokenPieces;

	// Delay in deleting the object
	bool timeToDie = false;
	float timeLeft = 1.0f;

	void SpawnBrokenPieces(Mesh& mesh);

public:
	Breakable() { type = BreakableC; }
	
	void Break();

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_BREAKABLE_H_
