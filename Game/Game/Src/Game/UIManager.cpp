// @file: UIManager.cpp
//
// @brief: Cpp file for UIManager component, managing the UI of the game.

#include "stdafx.h"
#include "App/app.h"
#include "Game/UIManager.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Systems/RenderSystem.h"

void UIManager::Initialize()
{
	// Start the game with 10 balls
	ballsLeft = 10;

	// Starting message
	UIBuffer startMsg;
	startMsg.position.x = APP_VIRTUAL_WIDTH / 2 - 80;
	startMsg.position.y = APP_VIRTUAL_HEIGHT - 100;
	startMsg.timeRemaining = 2.0f;
	startMsg.color = Vector3(1.0f, 0.0f, 0.0f);
	startMsg.project = false;
	startMsg.text = "Click to Shoot Balls!";
	renderBuffer.push_back(startMsg);
}

void UIManager::Update(float deltaTime)
{
	// Check if any items from render buffer should be removed
	for (UIBuffer& uiBuffer : renderBuffer)
	{
		uiBuffer.timeRemaining -= (deltaTime / 1000.0f);
	}
	renderBuffer.remove_if([](const UIBuffer& uiB) { return (uiB.timeRemaining <= 0); });

	CheckForGamePause();
}

void UIManager::Render()
{
	// Render the text data present in renderBuffer list
	RenderTheBuffer();

	// Remaining balls
	App::Print(APP_VIRTUAL_WIDTH / 2 - 5, APP_VIRTUAL_HEIGHT - 50, std::to_string(ballsLeft).c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);
	if (isGamePaused)
		App::Print(APP_VIRTUAL_WIDTH / 2 - 80, APP_VIRTUAL_HEIGHT - 100, "GAME PAUSED", 1.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24);

	// Pause text
	if (isGamePaused)
		App::Print(APP_VIRTUAL_WIDTH - 190, APP_VIRTUAL_HEIGHT - 40, "Press P to Resume", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	else
		App::Print(APP_VIRTUAL_WIDTH - 170, APP_VIRTUAL_HEIGHT - 40, "Press P to Pause", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
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
		isGamePaused = !isGamePaused;
	}
	else if (!App::IsKeyPressed('P') && _pausekeyPressed)
	{
		_pausekeyPressed = false;
	}
}
