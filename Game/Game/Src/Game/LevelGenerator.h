// @file: LevelGenerator.h
//
// @brief: Header file for LevelGenerator class responsible for generating level as per the position of player.

#pragma once
#ifndef _LEVEL_GENERATOR_H_
#define _LEVEL_GENERATOR_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"

class Entity;
class BallSpawner;
class UIManager;
enum BreakableType;

class LevelGenerator : public Component
{
	Entity* ballSpawnerEntity = nullptr;
	BallSpawner* ballSpawner = nullptr;
	UIManager* uiManager = nullptr;

	const float SEPARATION_DIST = 50.0f;
	const float STAR_PROBABILITY = 0.35f;
	const float PLANE_PROBABILITY = 0.35f;
	const float PLANE_MOVE_PROBABILITY = 0.3f;

	// Player can't see beyond this distance, so don't spawn levels beyond this
	float maxSightDistance = 150.0f;
	float lastSpawnDistance = 0.0f;

	// To ensure that initial blocks are simpler
	int _countIter = 0;
	bool isFirstDoor = true;

	Entity* CreateWallEntity(Vector3& position, Vector3& scale, bool isDoor = false, bool opensLeft = false);
	Entity* CreateBreakableEntity(Vector3& position, Vector3& scale, Vector3& rotation, BreakableType breakableType);
	
	void SpawnLevel(float zPos);

public:
	LevelGenerator() { type = LevelGeneratorC; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_LEVEL_GENERATOR_H_
