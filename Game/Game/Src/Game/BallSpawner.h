// @file: BallSpawner.h
//
// @brief: Header file for ball spawner functionalities.

#pragma once
#ifndef _BALL_SPAWNER_H_
#define _BALL_SPAWNER_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Mesh.h"

class BallSpawner : public Component
{
	int ballCounter = 0;
	float ballSpeed = 100.0f;
	float spawnerMoveSpeed = 10.0f;

	// Ball component data
	std::string meshObjFile = "";
	Mesh mesh;
	std::vector<ComponentType> ballComponents{ MeshRendererC, BoxColliderC, RigidBodyC, BallC };

	// To avoid multiple clicks
	bool isClickPressed = false;

	// Create a new ball entity and shoot it
	void SpawnBall();

public:
	BallSpawner() { type = BallSpawnerC; }
	void SetMeshObj(const std::string& filename) { meshObjFile = filename; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_BALL_SPAWNER_H_
