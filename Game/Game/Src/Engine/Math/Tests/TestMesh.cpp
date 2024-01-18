// @file: TestMesh.cpp
//
// @brief: Cpp file for TestMesh class containing unit tests for Mesh class.

#include "stdafx.h"
#include "TestMesh.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Core/Logger.h"

void TestMesh::RunTests()
{
	TestLoadFromObjectFile();
	Logger::Get().Log("[UNITTEST] Mesh - All tests passed!");
}

void TestMesh::TestLoadFromObjectFile()
{
	Mesh mesh;
	mesh.LoadFromObjectFile("Assets/Objects/testObject.obj");
	assert(mesh.faces.size() == 12);
}
