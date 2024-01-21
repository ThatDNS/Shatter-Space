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
class DoorOpener;

enum BreakableType
{
	Pyramid,
	Plane,
	Star
};

class Breakable : public Component
{
	BreakableType breakableType = Pyramid;
	const int PYRAMID_SCORE = 1;
	const int PLANE_SCORE = 0;
	const int STAR_SCORE = 5;
	int _score = 0;

	// Cached components
	MeshRenderer* meshRenderer = nullptr;
	RigidBody* rigidBody = nullptr;
	Particles* particles = nullptr;
	UIManager* uiManager = nullptr;

	// Doors to open
	std::vector<DoorOpener*> doorOpeners;

	// Broken mesh parts
	std::list<Mesh> brokenPieces;

	// Is the breakable moving vertically? Used for plane.
	float amplitude = 10.0f;
	float theta = 0.0f;
	bool moveVertically = false;

	// Delay in deleting the object
	bool timeToDie = false;
	float timeLeft = 1.0f;

	void LoadMeshes(std::string& meshObjFile, size_t meshPieces);
	void SpawnBrokenPieces(Mesh& mesh);
	// Perform SHM movement along Y axis
	void SHMMovement(float);

public:
	Breakable() { type = BreakableC; }
	
	void Break(float updateScore = true);
	void SetBreakableType(BreakableType bt) { breakableType = bt; }
	void SetMoveVertically(bool move) { moveVertically = move; }
	void AttachDoorOpener(DoorOpener* door) { doorOpeners.push_back(door); }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_BREAKABLE_H_
