// @file: LevelGenerator.h
//
// @brief: Header file for LevelGenerator class responsible for generating level as per the position of player.

#pragma once
#ifndef _LEVEL_GENERATOR_H_
#define _LEVEL_GENERATOR_H_

#include "Engine/Components/Component.h"
#include "Engine/Math/Vector3.h"

class Entity;
enum BreakableType;

class LevelGenerator : public Component
{
	Entity* playerEntity = nullptr;

	const float SEPARATION_DIST = 50.0f;
	const float STAR_PROBABILITY = 0.6f;

	// Player can't see beyond this distance, so don't spawn levels beyond this
	float maxSightDistance = 150.0f;
	float lastSpawnDistance = 0.0f;

	int _pyramidCounter = 0;

	void CreateWallEntity(Vector3& position, Vector3& scale);
	void CreateBreakableEntity(Vector3& position, Vector3& scale, Vector3& rotation, BreakableType breakableType);
	
	void SpawnLevel(float zPos);

public:
	LevelGenerator() { type = LevelGeneratorC; }

	void Initialize() override;
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_LEVEL_GENERATOR_H_
