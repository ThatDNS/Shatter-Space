//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app/app.h"
//------------------------------------------------------------------------
#include "Engine/Core/Logger.h"
#include "Engine/Systems/Engine.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Triangle.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Math/EngineMath.h"
#include "Engine/Systems/RenderSystem.h"

extern void SetupLevel1();

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Wake up the engine
	Engine::Get().Wakeup();

	// Setup level 1
	SetupLevel1();

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
	App::Print(50, APP_INIT_WINDOW_HEIGHT-50, "Level 1");
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	Engine::Get().Destroy();
}
