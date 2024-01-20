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
}

void UIManager::Render()
{
	// Remaining balls
	App::Print(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT - 50,
		std::to_string(ballsLeft).c_str(), 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);
}
