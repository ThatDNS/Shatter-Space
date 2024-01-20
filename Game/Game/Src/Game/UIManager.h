// @file: UIManager.h
//
// @brief: Header file for UIManager component, managing the UI of the game.

#pragma once
#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "Engine/Components/Renderable.h"

struct UIBuffer
{
	// Not using Vector3 to save 4 bytes. Doesn't matter much but it makes me happy :)
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	// The text will get rendered for this much time
	float timeRemaining = 1.0f;  // in seconds

	// Should it be transform to projection space
	bool project = false;
	
	std::string text = "";
};

class UIManager : public Renderable
{
	int ballsLeft = 0;
	bool ballsChanged = false;

	bool _pausekeyPressed = false;
	bool isGamePaused = false;

	// Anything added to it gets rendered for the specified time
	std::list<UIBuffer> renderBuffer;

	void CheckForGamePause();

public:
	UIManager() { type = UIManagerC; }

	void Initialize() override;
	void Update(float) override;
	void Render() override;
	void Destroy() override {}

	void ScheduleRender(UIBuffer& uiB) { renderBuffer.push_back(uiB); }
	
	bool IsGamePaused() const { return isGamePaused; }
	int GetBallsLeft() const { return ballsLeft; }
	void IncreaseBalls(int n) { ballsLeft += n; ballsChanged = true; }
	void DecreaseBalls(int n) { ballsLeft -= n; ballsChanged = true; }
};

#endif // !_UIMANAGER_H_
