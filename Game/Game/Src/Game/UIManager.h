// @file: UIManager.h
//
// @brief: Header file for UIManager component, managing the UI of the game.

#pragma once
#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Math/Vector3.h"

struct UIBuffer
{
	Vector3 position{ 0.0f, 0.0f, 0.0f };
	Vector3 color{ 1.0f, 1.0f, 1.0f };

	// The text will get rendered for this much time
	float timeRemaining = 1.0f;  // in seconds

	// Should it be transform to projection space
	bool project = false;
	
	std::string text = "";
};

class UIManager : public Renderable
{
	const STRCODE highestScoreHash = 12345;
	std::string highscore = "0";

	// Toggle between the songs during gameplay
	// 0 : Lights
	// 1 : Level 1
	int songPlaying = 0;

	int ballsLeft = 0;
	bool ballsChanged = false;

	bool _pausekeyPressed = false;
	bool gameStarted = false;
	bool gamePaused = false;
	bool gameOver = false;
	bool isTutorialUp = false;

	// Timers for starting instruction messages
	float obstacles_msg_timer = 5.0f;
	float stars_msg_timer = 10.0f;
	float tutorial_timer = 0.0f;

	// Anything added to it gets rendered for the specified time
	std::list<UIBuffer> renderBuffer;

	void CheckForGameStart();
	void CheckForGamePause();
	void CheckForGameRestart();
	void RenderTheBuffer();
	void DisplayObstaclesMsg();
	void DisplayStarsMsg();
	void DisplayWelcomeMessage();

public:
	UIManager() { type = UIManagerC; }

	void Initialize() override;
	void Update(float) override;
	void Render() override;
	void Destroy() override;

	void ScheduleRender(UIBuffer& uiB);
	void SetTutorialTimer(float timer);

	bool HasGameStarted() const { return gameStarted; }
	bool IsGamePaused() const { return gamePaused; }
	bool IsGameOver() const { return gameOver; }
	int GetBallsLeft() const { return ballsLeft; }
	void IncreaseBalls(int n);
	void DecreaseBalls(int n);
};

#endif // !_UIMANAGER_H_
