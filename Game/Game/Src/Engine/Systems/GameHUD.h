// @file: GameHUD.h
//
// @brief: Header file for GameHUD, a singleton responsible for handling rending of text on the screen.

#pragma once
#ifndef _GAME_HUD_H_
#define _GAME_HUD_H_

struct HUDText
{
	// Position
	float x = 0.0f, y = 0.0f;
	std::string text = "";

	// Color
	float r = 1.0f, g = 1.0f, b = 1.0f;
};

class GameHUD
{
	DECLARE_SINGLETON(GameHUD)

	// Data scheduled to be rendered
	// Removed after getting rendered
	std::vector<HUDText> textsToRender;

protected:
	void Initialize() {}

	/**
	 * @brief Renders the HUD text.
	 */
	void Render();

	friend class Engine;
};

#endif // !_GAME_HUD_H_
