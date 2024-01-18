// @file: TestMatrix4x4.h
//
// @brief: Header file for TestMatrix4x4 class containing unit tests for Matrix4x4 class.

#pragma once
#ifndef _TEST_MATRIX4x4_H_
#define _TEST_MATRIX4x4_H_

class TestMatrix4x4
{
public:
	static void RunTests();

	static void TestCreateLookAt();
	static void TestCreatePerspectiveFieldOfView();
	static void TestCreateScale();
	static void TestCreateTranslation();
	static void TestCreateRotation();
	static void TestCreateRotationX();
	static void TestCreateRotationY();
	static void TestCreateRotationZ();
	static void TestIdentity();
	static void TestToString();
	static void TestOperatorOverloads();
};

#endif // !_TEST_MATRIX4x4_H_
