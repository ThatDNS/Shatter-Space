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
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Triangle.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Math/EngineMath.h"
#include "Engine/Systems/RenderSystem.h"

extern void Game_Register();

Mesh mesh;
Matrix4x4 matProj;
Matrix4x4 matView;
float rotTheta = 0.0f;

Vector3 vCamera;
Vector3 vLookDir;
float cameraYaw = 0.0f;  // Y rotation

Vector3 lightDirection{ 0.0f, 0.0f, -1.0f };
float moveSpeed = 0.05f;

float objZ = 0.0f;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Register type classes used in game code
	Game_Register();

	Engine::Get().Initialize();

	mesh.LoadFromObjectFile("Assets/mountains.obj");
	//matProj = Matrix4x4::CreatePerspectiveFieldOfView(90.0f, (float)APP_INIT_WINDOW_HEIGHT / (float)APP_INIT_WINDOW_WIDTH, 0.1f, 1000.0f);
	matProj = RenderSystem::Get().GetProjectionMatrix();
	lightDirection.Normalize();
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
	/*
	// Camera movement
	if (App::GetController(0).GetLeftThumbStickX() > 0.5f)
	{
		vCamera.x += deltaTime * moveSpeed;
	}
	if (App::GetController(0).GetLeftThumbStickX() < -0.5f)
	{
		vCamera.x -= deltaTime * moveSpeed;
	}
	if (App::GetController(0).GetLeftThumbStickY() > 0.5f)
	{
		vCamera.y += deltaTime * moveSpeed;
	}
	if (App::GetController(0).GetLeftThumbStickY() < -0.5f)
	{
		vCamera.y -= deltaTime * moveSpeed;
	}

	// For Z-axis movement, camera should move on look-at vector direction
	Vector3 vForward = vLookDir * (deltaTime * moveSpeed);
	if (App::GetController(0).GetRightThumbStickY() < -0.5f)
		vCamera += vForward;
	if (App::GetController(0).GetRightThumbStickY() > 0.5f)
		vCamera -= vForward;

	// Camera rotation
	if (App::GetController(0).GetRightThumbStickX() > 0.5f)
		cameraYaw += deltaTime / 100.0f;
	if (App::GetController(0).GetRightThumbStickX() < -0.5f)
		cameraYaw -= deltaTime / 100.0f;

	//rotTheta += deltaTime / 800.0f;
	objZ += deltaTime / 1000.0f;
	*/
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
	/*static float a = 0.0f;
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
	}*/
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	Engine::Get().Destroy();
}
