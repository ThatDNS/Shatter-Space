#include "stdafx.h"
#include "app/app.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/EngineMath.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Core/Logger.h"

void MeshRenderer::Initialize()
{
	// Ensure that the mesh is not hidden
	SetHideMesh(false);
}

void MeshRenderer::LoadMesh(const std::string& objFileLocation)
{
	mesh.LoadFromObjectFile(objFileLocation);
}

void MeshRenderer::LoadMesh(Mesh& _mesh)
{
	mesh = _mesh;
}

Matrix4x4 MeshRenderer::GetWorldMatrix()
{
	// Scale, Rotate, Translate
	Transform& transform = GetEntity()->GetTransform();
	return (Matrix4x4::CreateScale(transform.scale) *
		    Matrix4x4::CreateRotation(transform.rotation) *
			Matrix4x4::CreateTranslation(transform.position));
}

void MeshRenderer::Render()
{
	if (hideMesh)
		return;

	// World matrix
	Matrix4x4 mWorld = GetWorldMatrix();

	// View matrix
	Matrix4x4 mView = RenderSystem::Get().GetViewMatrix();

	// Projection matrix
	Matrix4x4 mProj = RenderSystem::Get().GetProjectionMatrix();

	Vector3 cameraPosition = RenderSystem::Get().GetCameraPosition();
	float cameraAbsZ = std::abs(cameraPosition.z);

	// Draw the mesh (Does world, view, projection transformation)
	// Triangle gets transformed so creating a copy in loop
	std::vector<std::pair<Triangle, float>> triToRaster;  // Not all triangles get rendered, filtering here
	for (Triangle tri : mesh.faces)
	{
		float lightIntensity = 1.0f;
		// 1.) Transform to world space
		tri.points[0] = mWorld * tri.points[0];
		tri.points[1] = mWorld * tri.points[1];
		tri.points[2] = mWorld * tri.points[2];

		// Triangle normal
		Vector3 line1 = tri.points[1] - tri.points[0];
		Vector3 line2 = tri.points[2] - tri.points[0];
		Vector3 normal = Vector3::Cross(line1, line2);
		normal.Normalize();

		if (!renderBackSide)
		{
			// Ray casted from triangle to camera
			if (Vector3::Dot(normal, (tri.points[0] - cameraPosition)) > 0)
				continue;
		}

		// Directional light
		if (RenderSystem::Get().HasSun())
		{
			lightIntensity = std::max(0.2f, Vector3::Dot(RenderSystem::Get().GetSunlightDirection(), normal));
		}
		else if (RenderSystem::Get().HasDepthShadow())
		{
			// zDelta - [0, 50]   -> alpha = 1.0
			// zDelta - [50, 200] -> alpha = [1.0, 0.0]
			float zDelta = std::abs(std::abs(tri.points[0].z) - std::abs(cameraAbsZ));
			zDelta = std::max(0.0f, zDelta - 50.0f);
			lightIntensity = (1.0f - zDelta / 150.0f);
		}

		// 2.) Transform to view space
		tri.points[0] = mView * tri.points[0];
		tri.points[1] = mView * tri.points[1];
		tri.points[2] = mView * tri.points[2];

		// Clip the view space triangle agaist near plane
		// This can form 2 new triangles (due to formation of a quad)
		Triangle clipped[2];
		int nClippedTriangles = ClipTriangleByPlane(Vector3(0.0f, 0.0f, 0.1f), Vector3(0.0f, 0.0f, 1.0f), tri, clipped[0], clipped[1]);
		for (size_t i = 0; i < nClippedTriangles; i++)
		{
			// 3.) Projection (3D -> 2D)
			clipped[i].points[0] = mProj * clipped[i].points[0];
			clipped[i].points[1] = mProj * clipped[i].points[1];
			clipped[i].points[2] = mProj * clipped[i].points[2];
			// Normalize projection coordinates
			clipped[i].points[0] /= clipped[i].points[0].w;
			clipped[i].points[1] /= clipped[i].points[1].w;
			clipped[i].points[2] /= clipped[i].points[2].w;

			// Scale the object
			// Projection matrix result is in [-1, 1]. Shift it to [0, 2] & scale to the screen.
			clipped[i].points[0] += 1.0f;
			clipped[i].points[1] += 1.0f;
			clipped[i].points[2] += 1.0f;
			clipped[i].points[0].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			clipped[i].points[0].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
			clipped[i].points[1].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			clipped[i].points[1].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
			clipped[i].points[2].x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			clipped[i].points[2].y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;

			triToRaster.push_back({clipped[i], lightIntensity});
		}
	}

	// Add line points to set to avoid redrawing any line
	std::set<std::pair<std::pair<std::pair<float, float>, std::pair<float, float>>, float>> linesToDraw;
	for (auto& triIntensityPair : triToRaster)
	{
		Triangle& tri = triIntensityPair.first;
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to 
		// ensure we only test new triangles generated against planes
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
				Triangle current = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane
				switch (p)
				{
				case 0: nTrisToAdd = ClipTriangleByPlane(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), current, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = ClipTriangleByPlane(Vector3(0.0f, (float)APP_INIT_WINDOW_HEIGHT - 1, 0.0f), Vector3(0.0f, -1.0f, 0.0f), current, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = ClipTriangleByPlane(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), current, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = ClipTriangleByPlane(Vector3((float)APP_INIT_WINDOW_WIDTH - 1, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), current, clipped[0], clipped[1]); break;
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
		for (Triangle& t : listTriangles)
		{
			linesToDraw.insert({ { {t.points[0].x, t.points[0].y}, {t.points[1].x, t.points[1].y} }, triIntensityPair.second });
			linesToDraw.insert({ { {t.points[0].x, t.points[0].y}, {t.points[2].x, t.points[2].y} }, triIntensityPair.second });
			linesToDraw.insert({ { {t.points[1].x, t.points[1].y}, {t.points[2].x, t.points[2].y} }, triIntensityPair.second });
		}
	}

	// Draw the lines
	for (auto& lineIntensityPair : linesToDraw)
	{
		auto& linePoints = lineIntensityPair.first;
		float intensity = lineIntensityPair.second;

		auto& firstPt = linePoints.first;
		auto& secondPt = linePoints.second;
		App::DrawLine(firstPt.first, firstPt.second, secondPt.first, secondPt.second, meshColor.x * intensity, meshColor.y * intensity, meshColor.z * intensity);
	}
}

void MeshRenderer::Destroy()
{
	// Empty the mesh
	mesh.faces.clear();
}
