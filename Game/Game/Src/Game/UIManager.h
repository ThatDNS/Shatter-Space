// @file: UIManager.h
//
// @brief: Header file for UIManager component, managing the UI of the game.

#pragma once

#include "Engine/Components/Renderable.h"

class Canvas;

class UIManager : public Renderable
{
	int ballsLeft = 0;
	bool ballsChanged = false;

	Canvas* canvas = nullptr;

public:
	UIManager() { type = UIManagerC; }

	void Initialize() override;
	void Update(float) override;
	void Render() override;
	void Destroy() override {}

	int GetBallsLeft() const { return ballsLeft; }
	void IncreaseBalls(int n) { ballsLeft += n; ballsChanged = true; }
	void DecreaseBalls(int n) { ballsLeft -= n; ballsChanged = true; }
};

