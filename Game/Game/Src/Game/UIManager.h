// @file: UIManager.h
//
// @brief: Header file for UIManager component, managing the UI of the game.

#pragma once

#include "Engine/Components/Renderable.h"

struct UIBuffer
{
	float x = 0.0f;
	float y = 0.0f;

	// Should it be transform to projection space
	bool project = false;
	
	std::string text = "";

	// The text will get rendered for this much time
	float timeRemaining = 1.0f;  // in seconds
};

class UIManager : public Renderable
{
	int ballsLeft = 0;
	bool ballsChanged = false;

	// Anything added to it gets rendered for the specified time
	std::list<UIBuffer> renderBuffer;

public:
	UIManager() { type = UIManagerC; }

	void Initialize() override;
	void Update(float) override;
	void Render() override;
	void Destroy() override {}

	void ScheduleRender(UIBuffer& uiB) { renderBuffer.push_back(uiB); }

	int GetBallsLeft() const { return ballsLeft; }
	void IncreaseBalls(int n) { ballsLeft += n; ballsChanged = true; }
	void DecreaseBalls(int n) { ballsLeft -= n; ballsChanged = true; }
};

