// @file: TestMatrix4x4.cpp
//
// @brief: Cpp file for TestMatrix4x4 class containing unit tests for Matrix4x4 class.

#include "stdafx.h"
#include "TestMatrix4x4.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Core/Logger.h"

void TestMatrix4x4::RunTests()
{
	TestCreateLookAt();
	TestCreatePerspectiveFieldOfView();
	TestCreateScale();
	TestCreateTranslation();
	TestCreateRotation();
	TestCreateRotationX();
	TestCreateRotationY();
	TestCreateRotationZ();
	TestIdentity();
	TestToString();
	TestOperatorOverloads();
	Logger::Get().Log("[UNITTEST] Matrix4x4 - All tests passed!");
}

void TestMatrix4x4::TestCreateLookAt()
{
	Vector3 cameraPosition{ 0.0f, 1.0f, -2.0f };
	Matrix4x4 lookAtMat = Matrix4x4::CreateLookAt(cameraPosition, Vector3(0.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f));
	Matrix4x4 ans;
	ans[0][0] = 0.99999994f;
	ans[1][1] = 0.989949465f;
	ans[1][2] = 0.141421348f;
	ans[2][1] = -0.141421363f;
	ans[2][2] = 0.989949465f;
	ans[3][1] = 1.0f;
	ans[3][2] = -2.0f;
	ans[3][3] = 1.0f;
	assert(lookAtMat == ans);
}

void TestMatrix4x4::TestCreatePerspectiveFieldOfView()
{
	Matrix4x4 projectionMat = Matrix4x4::CreatePerspectiveFieldOfView(60.0f, 0.75f, 1.0f, 500.0f);
	Matrix4x4 ans;
	ans[0][0] = 1.29903817f;
	ans[1][1] = 1.7320509f;
	ans[2][2] = 1.00200403f;
	ans[2][3] = 1.0f;
	ans[3][2] = -1.00200403f;
	assert(projectionMat == ans);
}

void TestMatrix4x4::TestCreateScale()
{
	Matrix4x4 scaleMat1 = Matrix4x4::CreateScale(Vector3(1.0f, 1.5f, 2.0f));
	Matrix4x4 scaleMat2 = Matrix4x4::CreateScale(1.0f, 1.5f, 2.0f);
	Matrix4x4 ans;
	ans[0][0] = 1.0f;
	ans[1][1] = 1.5f;
	ans[2][2] = 2.0f;
	ans[3][3] = 1.0f;
	assert(scaleMat1 == ans);
	assert(scaleMat2 == ans);
}

void TestMatrix4x4::TestCreateTranslation()
{
	Matrix4x4 translationMat1 = Matrix4x4::CreateTranslation(Vector3(-5.0f, 2.5f, -10.0f));
	Matrix4x4 translationMat2 = Matrix4x4::CreateTranslation(-5.0f, 2.5f, -10.0f);
	Matrix4x4 ans;
	ans[0][0] = 1.0f;
	ans[1][1] = 1.0f;
	ans[2][2] = 1.0f;
	ans[3][0] = -5.0f;
	ans[3][1] = 2.5f;
	ans[3][2] = -10.0f;
	ans[3][3] = 1.0f;
	assert(translationMat1 == ans);
	assert(translationMat2 == ans);
}

void TestMatrix4x4::TestCreateRotation()
{
	Matrix4x4 rotation = Matrix4x4::CreateRotation(Vector3(25.5f, 15.0f, 5.0f));
	Matrix4x4 ans;
	ans[0][0] = -0.215494737f;
	ans[0][1] = 0.7284832f;
	ans[0][2] = 0.650287867f;
	ans[1][0] = 0.828745842f;
	ans[1][1] = 0.488646686f;
	ans[1][2] = -0.272772282f;
	ans[2][0] = -0.516471028f;
	ans[2][1] = 0.480142355f;
	ans[2][2] = -0.709028184f;
	ans[3][3] = 1.0f;
	assert(rotation == ans);
}

void TestMatrix4x4::TestCreateRotationX()
{
	Matrix4x4 rotation = Matrix4x4::CreateRotationX(25.5f);
	Matrix4x4 ans;
	ans[0][0] = 1.0f;
	ans[1][1] = 0.933315098f;
	ans[1][2] = 0.359058350f;
	ans[2][1] = -0.359058350f;
	ans[2][2] = 0.933315098f;
	ans[3][3] = 1.0f;
	assert(rotation == ans);
}

void TestMatrix4x4::TestCreateRotationY()
{
	Matrix4x4 rotation = Matrix4x4::CreateRotationY(25.5f);
	Matrix4x4 ans;
	ans[0][0] = 0.933315098f;
	ans[0][2] = 0.359058350f;
	ans[1][1] = 1.0f;
	ans[2][0] = -0.359058350f;
	ans[2][2] = 0.933315098f;
	ans[3][3] = 1.0f;
	assert(rotation == ans);
}

void TestMatrix4x4::TestCreateRotationZ()
{
	Matrix4x4 rotation = Matrix4x4::CreateRotationZ(25.5f);
	Matrix4x4 ans;
	ans[0][0] = 0.933315098f;
	ans[0][1] = 0.359058350f;
	ans[1][0] = -0.359058350f;
	ans[1][1] = 0.933315098f;
	ans[2][2] = 1.0f;
	ans[3][3] = 1.0f;
	assert(rotation == ans);
}

void TestMatrix4x4::TestIdentity()
{
	Matrix4x4 identityMat = Matrix4x4::Identity();
	Matrix4x4 ans;
	ans[0][0] = 1.0f;
	ans[1][1] = 1.0f;
	ans[2][2] = 1.0f;
	ans[3][3] = 1.0f;
	assert(identityMat == ans);
}

void TestMatrix4x4::TestToString()
{
	Matrix4x4 mat;
	mat[0][0] = -0.215494737f;
	mat[0][1] = 0.7284832f;
	mat[0][2] = 0.650287867f;
	mat[1][0] = 0.828745842f;
	mat[1][1] = 0.488646686f;
	mat[1][2] = -0.272772282f;
	mat[2][0] = -0.516471028f;
	mat[2][1] = 0.480142355f;
	mat[2][2] = -0.709028184f;
	mat[3][3] = 1.0f;
	assert(mat.ToString() == "Matrix4x4( {-0.215495, 0.728483, 0.650288, 0.000000 }, {0.828746, 0.488647, -0.272772, 0.000000 }, {-0.516471, 0.480142, -0.709028, 0.000000 }, {0.000000, 0.000000, 0.000000, 1.000000 }, )");
}

void TestMatrix4x4::TestOperatorOverloads()
{
	Matrix4x4 mat = Matrix4x4::Identity();
	mat[0][1] = 2.5f;
	mat[1][0] = 0.5f;
	mat[1][3] = -1.75f;
	mat[3][1] = 9.75f;

	// Matrix & Vector multiplication
	Vector3 multiResult =  mat * Vector3(1.0f, 0.5f, 1.3f);
	Vector3 ans = Vector3(1.25f, 12.75f, 1.29999995f);
	ans.w = 0.125f;
	assert(multiResult == ans);

	// Matrix & Matrix multiplication
	Matrix4x4 mat1;
	mat1[0][0] = 1.2f;
	mat1[1][1] = 1.5f;
	mat1[2][2] = 2.0f;
	mat1[3][3] = 5.0f;
	Matrix4x4 result = mat * mat1;
	Matrix4x4 matAns;
	matAns[0][0] = 1.20000005f;
	matAns[0][1] = 3.75f;
	matAns[1][0] = 0.600000024f;
	matAns[1][1] = 1.5f;
	matAns[1][3] = -8.75f;
	matAns[2][2] = 2.0f;
	matAns[3][1] = 14.625f;
	matAns[3][3] = 5.0f;
	assert(result == matAns);
}
