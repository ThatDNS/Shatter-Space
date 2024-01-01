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

extern void Game_Register();

struct triangle
{
	Vector3 p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

mesh meshCube;
Matrix4x4 matProj;
float rotTheta = 0.0f;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Register type classes used in game code
	Game_Register();

	Engine::Get().Initialize();

	meshCube.tris = {

		// SOUTH
		{ Vector3(0.0f, 0.0f, 0.0f),    Vector3(0.0f, 1.0f, 0.0f),    Vector3(1.0f, 1.0f, 0.0f) },
		{ Vector3(0.0f, 0.0f, 0.0f),    Vector3(1.0f, 1.0f, 0.0f),    Vector3(1.0f, 0.0f, 0.0f) },

		// EAST                                                      
		{ Vector3(1.0f, 0.0f, 0.0f),    Vector3(1.0f, 1.0f, 0.0f),    Vector3(1.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, 0.0f, 0.0f),    Vector3(1.0f, 1.0f, 1.0f),    Vector3(1.0f, 0.0f, 1.0f) },

		// NORTH                                                     
		{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(1.0f, 1.0f, 1.0f),    Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(0.0f, 1.0f, 1.0f),    Vector3(0.0f, 0.0f, 1.0f) },

		// WEST                                                      
		{ Vector3(0.0f, 0.0f, 1.0f),    Vector3(0.0f, 1.0f, 1.0f),    Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(0.0f, 0.0f, 1.0f),    Vector3(0.0f, 1.0f, 0.0f),    Vector3(0.0f, 0.0f, 0.0f) },

		// TOP                                                       
		{ Vector3(0.0f, 1.0f, 0.0f),    Vector3(0.0f, 1.0f, 1.0f),    Vector3(1.0f, 1.0f, 1.0f) },
		{ Vector3(0.0f, 1.0f, 0.0f),    Vector3(1.0f, 1.0f, 1.0f),    Vector3(1.0f, 1.0f, 0.0f) },

		// BOTTOM                                                    
		{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(0.0f, 0.0f, 1.0f),    Vector3(0.0f, 0.0f, 0.0f) },
		{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(0.0f, 0.0f, 0.0f),    Vector3(1.0f, 0.0f, 0.0f) },

	};

	matProj = Matrix4x4::CreatePerspectiveFieldOfView(90.0f, (float)APP_INIT_WINDOW_HEIGHT / (float)APP_INIT_WINDOW_WIDTH, 0.1f, 1000.0f);
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

	rotTheta += deltaTime / 800.0f;
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

	// Draw the cube
	for (auto tri : meshCube.tris)
	{
		// Rotate in Z-axis
		tri.p[0] = Matrix4x4::CreateRotationZ(rotTheta) * tri.p[0];
		tri.p[1] = Matrix4x4::CreateRotationZ(rotTheta) * tri.p[1];
		tri.p[2] = Matrix4x4::CreateRotationZ(rotTheta) * tri.p[2];

		// Rotate in X-axis
		tri.p[0] = Matrix4x4::CreateRotationX(rotTheta * 0.5f) * tri.p[0];
		tri.p[1] = Matrix4x4::CreateRotationX(rotTheta * 0.5f) * tri.p[1];
		tri.p[2] = Matrix4x4::CreateRotationX(rotTheta * 0.5f) * tri.p[2];

		// Offset into the screen
		tri.p[0].z += 3.0f;
		tri.p[1].z += 3.0f;
		tri.p[2].z += 3.0f;

		Vector3 line1, line2;
		line1.x = tri.p[1].x - tri.p[0].x;
		line1.y = tri.p[1].y - tri.p[0].y;
		line1.z = tri.p[1].z - tri.p[0].z;
		line2.x = tri.p[2].x - tri.p[0].x;
		line2.y = tri.p[2].y - tri.p[0].y;
		line2.z = tri.p[2].z - tri.p[0].z;
		Vector3 normal = Vector3::Cross(line1, line2);
		normal.Normalize();

		if (normal.z > 0)
			continue;

		// Projection
		tri.p[0] = matProj * tri.p[0];
		tri.p[1] = matProj * tri.p[1];
		tri.p[2] = matProj * tri.p[2];

		// Scale the cube
		tri.p[0].x += 1.0f; tri.p[0].y += 1.0f;
		tri.p[1].x += 1.0f; tri.p[1].y += 1.0f;
		tri.p[2].x += 1.0f; tri.p[2].y += 1.0f;
		tri.p[0].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		tri.p[0].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
		tri.p[1].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		tri.p[1].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
		tri.p[2].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		tri.p[2].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;

		App::DrawLine(tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, 1.0f, 1.0f, 1.0f);
		App::DrawLine(tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y, 1.0f, 1.0f, 1.0f);
		App::DrawLine(tri.p[2].x, tri.p[2].y, tri.p[0].x, tri.p[0].y, 1.0f, 1.0f, 1.0f);
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
