// @file: BoxCollider.cpp
//
// @brief: Cpp file for BoxCollider class. Checks for box collision.

#include "stdafx.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Systems/RenderSystem.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Core/Logger.h"
#include "app/app.h"

//void BoxCollider::Initialize()
//{
//	// Find the mesh renderer on this entity
//	Entity* entity = GetEntity();
//	if (!entity->HasComponent(MeshRendererC))
//	{
//		Logger::Get().Log("Box collider can not be added on an entity without mesh renderer!");
//	}
//	else
//	{
//		meshR = static_cast<MeshRenderer*>(entity->GetComponent(MeshRendererC));
//	}
//}

void BoxCollider::AttachMesh(MeshRenderer* mr)
{
	meshR = mr;
}

void BoxCollider::Callibrate()
{
	if (meshR == nullptr)
	{
		Logger::Get().Log("No mesh renderer found! Box collider callibration failed.");
		return;
	}

	const Mesh mesh = meshR->GetMesh();
	Matrix4x4 mWorld = meshR->GetWorldMatrix();

	// Iterate through all the vertices of this mesh to get the min & max coords
	bool initialized = false;
	for (const Triangle& triangle : mesh.faces)
	{
		for (Vector3 triVert : triangle.points)
		{
			// The mesh is in local space. Need to transform it to world space
			triVert = mWorld * triVert;

			if (!initialized)
			{
				minCoords.x = triVert.x;
				minCoords.y = triVert.y;
				minCoords.z = triVert.z;

				maxCoords.x = triVert.x;
				maxCoords.y = triVert.y;
				maxCoords.z = triVert.z;
				initialized = true;
			}
			else
			{
				minCoords.x = std::min(minCoords.x, triVert.x);
				minCoords.y = std::min(minCoords.y, triVert.y);
				minCoords.z = std::min(minCoords.z, triVert.z);

				maxCoords.x = std::max(maxCoords.x, triVert.x);
				maxCoords.y = std::max(maxCoords.y, triVert.y);
				maxCoords.z = std::max(maxCoords.z, triVert.z);
			}
		}
	}
}

//bool BoxCollider::DidCollide(Collider* collider)
//{
//	// Get position of input collider
//	Vector3 targetPos = collider->GetEntity()->GetTransform().position;
//	return WillCollide(collider, targetPos);
//}
//
//bool BoxCollider::WillCollide(Collider* collider, Vector3& newPos)
//{
//	if (collider->GetColliderType() == BOX)
//	{
//		// Edges of first box
//	}
//
//	// Currently supporting only BOX to BOX collisions
//	return false;
//}

void BoxCollider::Draw()
{
	// Shorter names
	Vector3& a = minCoords;
	Vector3& b = maxCoords;

	std::vector<Vector3> points{
		Vector3(a.x, a.y, a.z),
		Vector3(b.x, a.y, a.z),
		Vector3(a.x, b.y, a.z),
		Vector3(b.x, b.y, a.z),
		Vector3(a.x, a.y, b.z),
		Vector3(b.x, a.y, b.z),
		Vector3(a.x, b.y, b.z),
		Vector3(b.x, b.y, b.z)
	};

	// View matrix
	Matrix4x4 mView = RenderSystem::Get().GetViewMatrix();

	// Projection matrix
	Matrix4x4 mProj = RenderSystem::Get().GetProjectionMatrix();

	for (Vector3& point : points)
	{
		// 1.) Transform to world space
		// The points were already transformed to world space in Callibrate.
		// No need of doing it again

		// 2.) Transform to view space
		point = mView * point;

		// 3.) Projection (3D -> 2D)
		point = mProj * point;
		// Normalize projection coordinates
		point /= point.w;

		// Scale
		point += 1.0f;
		point.x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
		point.y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
	}

	// Draw the lines representing the cuboid (in red)
	App::DrawLine(points[0].x, points[0].y, points[1].x, points[1].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[0].x, points[0].y, points[2].x, points[2].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[2].x, points[2].y, points[3].x, points[3].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[1].x, points[1].y, points[3].x, points[3].y, 0.5f, 0.0f, 0.0f);

	App::DrawLine(points[4].x, points[4].y, points[5].x, points[5].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[4].x, points[4].y, points[6].x, points[6].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[6].x, points[6].y, points[7].x, points[7].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[5].x, points[5].y, points[7].x, points[7].y, 0.5f, 0.0f, 0.0f);

	App::DrawLine(points[0].x, points[0].y, points[4].x, points[4].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[1].x, points[1].y, points[5].x, points[5].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[2].x, points[2].y, points[6].x, points[6].y, 0.5f, 0.0f, 0.0f);
	App::DrawLine(points[3].x, points[3].y, points[7].x, points[7].y, 0.5f, 0.0f, 0.0f);
}
