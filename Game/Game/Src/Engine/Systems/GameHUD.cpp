// @file: GameHUD.h
//
// @brief: Header file for GameHUD, a singleton responsible for handling rending of text on the screen.

#include "stdafx.h"
#include "App/app.h"
#include "Engine/Systems/GameHUD.h"

void GameHUD::Render()
{
	for (HUDText& data : textsToRender)
	{
		App::Print(data.x, data.y, data.text.c_str(), data.r, data.g, data.b);
	}
	textsToRender.clear();
}
