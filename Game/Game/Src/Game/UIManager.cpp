// @file: UIManager.cpp
//
// @brief: Cpp file for UIManager component, managing the UI of the game.

#include "stdafx.h"
#include "App/app.h"
#include "Game/UIManager.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

void UIManager::Initialize()
{
	// Start the game with 10 balls
	ballsLeft = 10;
	tutorial_timer = 0.0f;
	isTutorialUp = false;

	// Starting messages
	UIBuffer startMsg;
	startMsg.position.x = APP_VIRTUAL_WIDTH / 2 - 85;
	startMsg.position.y = APP_VIRTUAL_HEIGHT - 80;
	startMsg.timeRemaining = 4.0f;
	startMsg.color = Vector3(0.0f, 1.0f, 0.0f);
	startMsg.project = false;
	startMsg.text = "Click to Shoot Balls!";
	renderBuffer.push_back(startMsg);
	isTutorialUp = true;
	tutorial_timer += 4.0f;

	gameStarted = false;
	gamePaused = false;
	gameOver = false;

	if (SceneManager::Get().HasPersistentData(highestScoreHash))
	{
		highscore = SceneManager::Get().GetPersistentData(highestScoreHash);
	}

	obstacles_msg_timer = 5.0f;
	stars_msg_timer = 10.0f;
}

void UIManager::Update(float deltaTime)
{
	if (!gameStarted)
	{
		CheckForGameStart();
		return;
	}

	if (!gamePaused)
	{
		// To prevent rendering other messages if tutorial is running
		if (isTutorialUp)
		{
			tutorial_timer -= (deltaTime / 1000.0f);
			if (tutorial_timer <= 0)
			{
				tutorial_timer = 0.0f;
				isTutorialUp = false;
			}
		}
		// Check if any items from render buffer should be removed
		for (UIBuffer& uiBuffer : renderBuffer)
		{
			uiBuffer.timeRemaining -= (deltaTime / 1000.0f);
		}
		renderBuffer.remove_if([](const UIBuffer& uiB) { return (uiB.timeRemaining <= 0); });

		if (obstacles_msg_timer > 0.0f)
		{
			obstacles_msg_timer -= (deltaTime / 1000.0f);
			if (obstacles_msg_timer <= 0)
				DisplayObstaclesMsg();
		}
		if (stars_msg_timer > 0.0f)
		{
			stars_msg_timer -= (deltaTime / 1000.0f);
			if (stars_msg_timer <= 0)
				DisplayStarsMsg();
		}
	}

	if (gameOver)
		CheckForGameRestart();
	else
		CheckForGamePause();
}

void UIManager::Render()
{
	if (!gameStarted)
	{
		DisplayWelcomeMessage();
		return;
	}

	// Render the text data present in renderBuffer list
	RenderTheBuffer();

	// Distance covered
	float distance = std::abs(RenderSystem::Get().GetCameraPosition().z);
	std::string distStr = "Distance: " + std::to_string(static_cast<int>(distance));
	App::Print(20, APP_VIRTUAL_HEIGHT - 30, distStr.c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);

	// Highscore
	std::string highStr = "Highscore: " + highscore;
	App::Print(20, APP_VIRTUAL_HEIGHT - 60, highStr.c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);

	// Remaining balls
	std::string ballsLeftStr = "Balls Left: " + std::to_string(ballsLeft);
	if (ballsLeft <= 3)
		App::Print(APP_VIRTUAL_WIDTH / 2 - 50, APP_VIRTUAL_HEIGHT - 30, ballsLeftStr.c_str(), 1.0f, 0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18);
	else
		App::Print(APP_VIRTUAL_WIDTH / 2 - 50, APP_VIRTUAL_HEIGHT - 30, ballsLeftStr.c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);

	// Pause / Game Over text
	if (gameOver)
	{
		App::Print(APP_VIRTUAL_WIDTH / 2 - 70, APP_VIRTUAL_HEIGHT - 80, "GAME OVER", 1.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24);
		App::Print(APP_VIRTUAL_WIDTH / 2 - 70, APP_VIRTUAL_HEIGHT - 120, "Press R to Restart", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	}
	else
	{
		if (gamePaused)
		{
			App::Print(APP_VIRTUAL_WIDTH / 2 - 88, APP_VIRTUAL_HEIGHT - 120, "GAME PAUSED", 1.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24);
			App::Print(APP_VIRTUAL_WIDTH - 190, APP_VIRTUAL_HEIGHT - 30, "Press P to Resume", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		}
		else
		{
			App::Print(APP_VIRTUAL_WIDTH - 170, APP_VIRTUAL_HEIGHT - 30, "Press P to Pause", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		}
	}
}

void UIManager::RenderTheBuffer()
{
	// View projection
	Matrix4x4 mView = RenderSystem::Get().GetViewMatrix();

	// Projection matrix
	Matrix4x4 mProj = RenderSystem::Get().GetProjectionMatrix();

	// To set the font size
	float cameraPosZ = RenderSystem::Get().GetCameraPosition().z;

	// Render from the buffer
	for (UIBuffer& uiB : renderBuffer)
	{
		Vector3 point = uiB.position;
		if (uiB.project)
		{
			// Transform to view space
			point = mView * point;
			// Projection
			point = mProj * point;
			// Normalize
			point /= point.w;
			// Scale
			point += 1.0f;
			point.x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			point.y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;

			// Giving depth perception via font size
			float distFromCamera = std::abs(std::abs(uiB.position.z) - std::abs(cameraPosZ));
			if (distFromCamera > 100.0f)
				App::Print(point.x, point.y, uiB.text.c_str(), uiB.color.x, uiB.color.y, uiB.color.z, GLUT_BITMAP_HELVETICA_10);
			else if (distFromCamera > 50.0f)
				App::Print(point.x, point.y, uiB.text.c_str(), uiB.color.x, uiB.color.y, uiB.color.z, GLUT_BITMAP_HELVETICA_12);
			else
				App::Print(point.x, point.y, uiB.text.c_str(), uiB.color.x, uiB.color.y, uiB.color.z, GLUT_BITMAP_HELVETICA_18);
		}
		else
		{
			App::Print(point.x, point.y, uiB.text.c_str(), uiB.color.x, uiB.color.y, uiB.color.z, GLUT_BITMAP_HELVETICA_18);

		}
	}
}

void UIManager::CheckForGamePause()
{
	if (App::IsKeyPressed('P') && !_pausekeyPressed)
	{
		_pausekeyPressed = true;
		gamePaused = !gamePaused;
	}
	else if (!App::IsKeyPressed('P') && _pausekeyPressed)
	{
		_pausekeyPressed = false;
	}
}

void UIManager::CheckForGameRestart()
{
	if (App::IsKeyPressed('R'))
	{
		// Scene Reload
		SceneManager::Get().GetActiveScene()->ReloadScene();
	}
}

void UIManager::CheckForGameStart()
{
	if (App::IsKeyPressed(VK_SPACE))
	{
		gameStarted = true;
	}
}

void UIManager::IncreaseBalls(int n)
{
	if (gameOver)
		return;

	ballsLeft += n;
	ballsChanged = true;
}

void UIManager::DecreaseBalls(int n)
{
	ballsLeft -= n;
	ballsChanged = true;

	if (ballsLeft <= 0)
	{
		ballsLeft = 0;
		gameOver = true;

		int distance = static_cast<int>(std::abs(RenderSystem::Get().GetCameraPosition().z));
		// Update high score
		if (std::stoi(highscore) < distance)
		{
			highscore = std::to_string(distance);
			SceneManager::Get().StorePersistentData(highestScoreHash, highscore);
		}
	}
}

void UIManager::ScheduleRender(UIBuffer& uiB)
{
	if (!isTutorialUp)
		renderBuffer.push_back(uiB);
}

void UIManager::SetTutorialTimer(float timer)
{
	tutorial_timer += timer;
	isTutorialUp = true;
}

void UIManager::DisplayObstaclesMsg()
{
	UIBuffer startMsg;
	startMsg.position.x = APP_VIRTUAL_WIDTH / 2 - 135;
	startMsg.position.y = APP_VIRTUAL_HEIGHT - 80;
	startMsg.timeRemaining = 4.0f;
	startMsg.color = Vector3(0.0f, 0.8f, 1.0f);
	startMsg.project = false;
	startMsg.text = "Ice blocks can hurt you. Break them!";
	renderBuffer.push_back(startMsg);
	isTutorialUp = true;
	tutorial_timer += 4.0f;
}

void UIManager::DisplayStarsMsg()
{
	UIBuffer startMsg;
	startMsg.position.x = APP_VIRTUAL_WIDTH / 2 - 120;
	startMsg.position.y = APP_VIRTUAL_HEIGHT - 80;
	startMsg.timeRemaining = 4.0f;
	startMsg.color = Vector3(1.0f, 1.0f, 0.0f);
	startMsg.project = false;
	startMsg.text = "Always shoot for the STARS!";
	renderBuffer.push_back(startMsg);
	isTutorialUp = true;
	tutorial_timer += 4.0f;
}

void UIManager::DisplayWelcomeMessage()
{
	App::Print(APP_VIRTUAL_WIDTH / 2 - 70, APP_VIRTUAL_HEIGHT - 60, "SHATTER SPACE", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	App::Print(APP_VIRTUAL_WIDTH / 2 - 105, APP_VIRTUAL_HEIGHT - 120, "Use Mouse to Aim and Fire", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	App::Print(APP_VIRTUAL_WIDTH / 2 - 85, APP_VIRTUAL_HEIGHT - 180, "Press SPACE to Start", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	App::Print(APP_VIRTUAL_WIDTH / 2 - 20, APP_VIRTUAL_HEIGHT - 240, "Enjoy!", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
}
