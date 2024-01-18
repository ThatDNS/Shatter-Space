// @file: TestVector3.h
//
// @brief: Header file for TestVector3 class containing unit tests for Vector3 class.

#pragma once
#ifndef _TEST_VECTOR3_
#define _TEST_VECTOR3_

class TestVector3
{
public:
	static void RunTests();

	static void TestConstructor();
	static void TestMagnitude();
	static void TestNormalize();
	static void TestReset();
	static void TestDistance();
	static void TestDotProduct();
	static void TestCrossProduct();
	static void TestLerp();
	static void TestToString();
	static void TestOperatorOverloads();
};

#endif // !_TEST_VECTOR3_
