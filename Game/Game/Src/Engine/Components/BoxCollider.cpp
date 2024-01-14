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

void BoxCollider::Initialize()
{
	// Find the mesh renderer on this entity
	Entity* entity = GetEntity();
	if (!entity->HasComponent(MeshRendererC))
	{
		Logger::Get().Log("Box collider can not be added on an entity without mesh renderer!");
	}
	else
	{
		meshR = static_cast<MeshRenderer*>(entity->GetComponent(MeshRendererC));
	}
}

void BoxCollider::Update(float deltaTime)
{
	// Box collider must be reconstructed on every update as it can change if the
	// transform of this entity changes (as it changes the mesh's position/rotation/scale).
	Callibrate();
}

void BoxCollider::Callibrate()
{
	if (meshR == nullptr)
	{
		Logger::Get().Log("No mesh renderer found! Box collider callibration failed.");
		return;
	}

	const Mesh mesh = meshR->GetMesh();
	if (mesh.faces.size() == 0)
		return;

	Matrix4x4 mWorld = meshR->GetWorldMatrix();

	Vector3 minC(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector3 maxC(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

	// Iterate through all the vertices of this mesh to get the min & max coords
	for (const Triangle& triangle : mesh.faces)
	{
		for (Vector3 triVert : triangle.points)
		{
			// The mesh is in local space. Need to transform it to world space
			triVert = mWorld * triVert;
			
			minC.x = std::min(minC.x, triVert.x);
			minC.y = std::min(minC.y, triVert.y);
			minC.z = std::min(minC.z, triVert.z);

			maxC.x = std::max(maxC.x, triVert.x);
			maxC.y = std::max(maxC.y, triVert.y);
			maxC.z = std::max(maxC.z, triVert.z);
		}
	}
	boundingBox.minCoords = minC;
	boundingBox.maxCoords = maxC;
}

void BoxCollider::Render()
{
	if (!shouldRender)
		return;

	// Shorter names
	Vector3& a = boundingBox.minCoords;
	Vector3& b = boundingBox.maxCoords;

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

bool BoxCollider::DidCollide(Collider* collider)
{
	// Supporting only BOX-BOX collisions for now
	if (collider->GetColliderType() != BOX)
		return false;

	BoxCollider* boxC = static_cast<BoxCollider*>(collider);

	// Intersection b/w 2 AABBs
	return boundingBox.Intersects(boxC->boundingBox);
}
