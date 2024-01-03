#include "stdafx.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Math/Matrix4x4.h"

IMPLEMENT_DYNAMIC_CLASS(MeshRenderer)

void MeshRenderer::Load(json::JSON& mJSON)
{
	if (mJSON.hasKey("ObjectFile"))
	{
		mesh.LoadFromObjectFile(mJSON["ObjectFile"].ToString());
	}
}

void MeshRenderer::Initialize()
{
	// Cache transform
	transform = GetEntity()->GetTransform();
}

//Matrix4x4 MeshRenderer::GetWorldMatrix()
//{
//
//}

void MeshRenderer::Render()
{
	// World matrix

	// View matrix

	// Projection

}
