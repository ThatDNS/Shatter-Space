// @file: UIManager.cpp
//
// @brief: Cpp file for UIManager component, managing the UI of the game.

#include "stdafx.h"
#include "App/app.h"
#include "Game/UIManager.h"

void UIManager::Initialize()
{
	// Start the game with 10 balls
	ballsLeft = 10;
}

void UIManager::Update(float deltaTime)
{
	// Check if any items from render buffer should be removed
	for (UIBuffer& uiBuffer : renderBuffer)
	{
		uiBuffer.timeRemaining -= (deltaTime / 1000.0f);
	}
	renderBuffer.remove_if([](const UIBuffer& uiB) { return (uiB.timeRemaining <= 0); });
}

void UIManager::Render()
{
	// Render from the buffer
	for (UIBuffer& uiB : renderBuffer)
	{
		App::Print(uiB.x, uiB.y, uiB.text.c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_12);
	}

	// Remaining balls
	App::Print(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT - 50,
		std::to_string(ballsLeft).c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);
}
