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
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	//Engine::Get().Render();

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
	Matrix4x4 mTrans = Matrix4x4::CreateTranslation(0.0f, 0.0f, objZ);

	Matrix4x4 mWorld = (mRotZ * mRotX) * mTrans;

	Vector3 vUp = { 0, 1, 0 };
	// Camera by default points to Z-direction
	Vector3 vTarget = { 0, 0, 1 };
	// Player rotated in Y direction, rotate camera too
	Matrix4x4 matCameraRot = Matrix4x4::CreateRotationX(cameraYaw);
	vLookDir = matCameraRot * vTarget;
	vTarget = vCamera + vLookDir;
	
	matView = Matrix4x4::CreateLookAt(vCamera, vTarget, vUp);

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
		/*if (Vector3::Dot(normal, (tri.points[0] - vCamera)) > 0)
			continue;*/

		// Illumination
		//float lightIntensity = std::max(1.0f, Vector3::Dot(normal, lightDirection));

		// World space -> View space
		tri.points[0] = matView * tri.points[0];
		tri.points[1] = matView * tri.points[1];
		tri.points[2] = matView * tri.points[2];

		// Clip the view space triangle agaist near plane
		// This can form 2 new triangles (due to formation of a quad)
		Triangle clipped[2];
		int nClippedTriangles = ClipTriangleByPlane(Vector3(0.0f, 0.0f, 0.1f), Vector3(0.0f, 0.0f, 1.0f), tri, clipped[0], clipped[1]);

		for (size_t i = 0; i < nClippedTriangles; i++)
		{
			// Projection (3D -> 2D)
			clipped[i].points[0] = matProj * clipped[i].points[0];
			clipped[i].points[1] = matProj * clipped[i].points[1];
			clipped[i].points[2] = matProj * clipped[i].points[2];

			// Normalize projection coordinates
			clipped[i].points[0] /= clipped[i].points[0].w;
			clipped[i].points[1] /= clipped[i].points[1].w;
			clipped[i].points[2] /= clipped[i].points[2].w;

			// Scale the object
			clipped[i].points[0] += 1.0f;
			clipped[i].points[1] += 1.0f;
			clipped[i].points[2] += 1.0f;
			clipped[i].points[0].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			clipped[i].points[0].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
			clipped[i].points[1].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			clipped[i].points[1].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
			clipped[i].points[2].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			clipped[i].points[2].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;

			triToRaster.push_back(clipped[i]);
		}
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
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to 
		//  ensure we only test new triangles generated against planes
		Triangle clipped[2];
		std::list<Triangle> listTriangles;

		// Add initial triangle
		listTriangles.push_back(tri);
		size_t nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				// Take triangle from front of queue
				Triangle test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	nTrisToAdd = ClipTriangleByPlane(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = ClipTriangleByPlane(Vector3(0.0f, (float)APP_INIT_WINDOW_HEIGHT - 1, 0.0f), Vector3(0.0f, -1.0f, 0.0f), test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = ClipTriangleByPlane(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = ClipTriangleByPlane(Vector3((float)APP_INIT_WINDOW_WIDTH - 1, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), test, clipped[0], clipped[1]); break;
				}

				// Clipping may yield a variable number of triangles, so
				// add these new ones to the back of the queue for subsequent
				// clipping against next planes
				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}

		// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto& t : listTriangles)
		{
			App::DrawLine(t.points[0].x, t.points[0].y, t.points[1].x, t.points[1].y, 1.0f, 1.0f, 1.0f);
		}
		/*
		App::DrawLine(tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, 1.0f, 1.0f, 1.0f);
		App::DrawLine(tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, 1.0f, 1.0f, 1.0f);
		App::DrawLine(tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, 1.0f, 1.0f, 1.0f);*/
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
