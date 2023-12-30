//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Engine/Core/Logger.h"
#include "Engine/Systems/Engine.h"

extern void Game_Register();

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Register type classes used in game code
	Game_Register();

	Engine::Get().Initialize();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	Engine::Get().Update(deltaTime);
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound("Assets/TestData/Test.wav");
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	Engine::Get().Render();

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f)*60.0f;
		float sy = 200 + cosf(a + i * 0.1f)*60.0f;
		float ex = 700 - sinf(a + i * 0.1f)*60.0f;
		float ey = 700 - cosf(a + i * 0.1f)*60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		App::DrawLine(sx, sy, ex, ey,r,g,b);
	}
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	Engine::Get().Destroy();
}
