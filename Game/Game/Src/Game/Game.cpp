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

extern void Game_Register();

Mesh mesh;
Matrix4x4 matProj;
float rotTheta = 0.0f;
Vector3 vCamera;
Vector3 lightDirection{ 0.0f, 0.0f, -1.0f };

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Register type classes used in game code
	Game_Register();

	Engine::Get().Initialize();

	mesh.LoadFromObjectFile("Assets/VideoShip.obj");
	matProj = Matrix4x4::CreatePerspectiveFieldOfView(90.0f, (float)APP_INIT_WINDOW_HEIGHT / (float)APP_INIT_WINDOW_WIDTH, 0.1f, 1000.0f);
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

	// World matrix (Scale, Rotate, Translate)
	Matrix4x4 mRotZ = Matrix4x4::CreateRotationZ(rotTheta);
	Matrix4x4 mRotX = Matrix4x4::CreateRotationX(rotTheta * 0.5f);
	Matrix4x4 mTrans = Matrix4x4::CreateTranslation(0.0f, 0.0f, 8.0f);

	Matrix4x4 mWorld = (mRotZ * mRotX) * mTrans;

	std::vector<Triangle> triToRaster;
	// Draw the mesh
	for (Triangle tri : mesh.faces)
	{
		// Transform to world
		tri.points[0] = mWorld * tri.points[0];
		tri.points[1] = mWorld * tri.points[1];
		tri.points[2] = mWorld * tri.points[2];

		// Compute normals (to determine what is facing camera)
		Vector3 line1 = tri.points[1] - tri.points[0];
		Vector3 line2 = tri.points[2] - tri.points[0];
		Vector3 normal = Vector3::Cross(line1, line2);
		normal.Normalize();

		// Ray casted from triangle to camera
		if (Vector3::Dot(normal, (tri.points[0] - vCamera)) > 0)
			continue;

		// Illumination
		float lightIntensity = std::max(1.0f, Vector3::Dot(normal, lightDirection));

		// Projection
		tri.points[0] = matProj * tri.points[0];
		tri.points[1] = matProj * tri.points[1];
		tri.points[2] = matProj * tri.points[2];

		// Normalize projection coordinates
		tri.points[0] /= tri.points[0].w;
		tri.points[1] /= tri.points[1].w;
		tri.points[2] /= tri.points[2].w;

		// Scale the cube
		tri.points[0] += 1.0f;
		tri.points[1] += 1.0f;
		tri.points[2] += 1.0f;
		tri.points[0].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		tri.points[0].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
		tri.points[1].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		tri.points[1].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
		tri.points[2].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		tri.points[2].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;

		triToRaster.push_back(tri);
	}

	// Sort triangles from back to front (THIS DOES NOT MATTER IN WIREFRAME??)
	std::sort(triToRaster.begin(), triToRaster.end(), [](Triangle& t1, Triangle& t2)
		{
			// Get mid point of Z of triangle
			float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
			float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
			return z1 > z2;
		});

	for (Triangle& tri : triToRaster)
	{
		App::DrawLine(tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, 1.0f, 1.0f, 1.0f);
		App::DrawLine(tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, 1.0f, 1.0f, 1.0f);
		App::DrawLine(tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, 1.0f, 1.0f, 1.0f);
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
