// @file: TestBVH.cpp
//
// @brief: Cpp file for TestBVH class containing unit tests for BVH (Bounding Volume Hierarchy) class.

#include "stdafx.h"
#include "TestBVH.h"
#include "Engine/Algorithms/BVH.h"
#include "Engine/Algorithms/AABB.h"
#include "Engine/Core/Logger.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Math/Random.h"

void TestBVH::RunTests()
{
	BVH* bvhTree = new BVH();

	// Sample box colliders
	std::vector<BoxCollider*> boxColliders;
	for (size_t i = 0; i < 20; i++)
	{
		BoxCollider* boxC = new BoxCollider();
		
		Vector3 minC{ Random::Get().Float() * 100.0f,
					Random::Get().Float() * 100.0f,
					Random::Get().Float() * 100.0f };
		Vector3 maxC{ minC.x + Random::Get().Float() * 10.0f,
					minC.y + Random::Get().Float() * 10.0f,
					minC.z + Random::Get().Float() * 10.0f };
		boxC->boundingBox = AABB(minC, maxC);

		boxColliders.push_back(boxC);
	}

	TestBuildTree(bvhTree, boxColliders);
	TestCheckCollisions(bvhTree);
	TestDestroy(bvhTree);
	Logger::Get().Log("[UNITTEST] BVH - All tests passed!");

	// Don't forget to free up the memory :)
	for (BoxCollider* boxC : boxColliders)
	{
		delete boxC;
	}
}

void TestBVH::TestBuildTree(BVH* bvhTree, std::vector<BoxCollider*>& boxColliders)
{
	assert(bvhTree->root == nullptr);
	bvhTree->BuildTree(boxColliders);
	assert(bvhTree->root != nullptr);
}

void TestBVH::TestCheckCollisions(BVH* bvhTree)
{
	BoxCollider* boxC = new BoxCollider();
	boxC->boundingBox = AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f));
	assert(bvhTree->CheckCollisions(boxC));
}

void TestBVH::TestDestroy(BVH* bvhTree)
{
	assert(bvhTree->root != nullptr);
	bvhTree->Destroy();
	assert(bvhTree->root == nullptr);
}
