// @file: TestAABB.h
//
// @brief: Header file for TestAABB class containing unit tests for AABB class.

#pragma once
#ifndef _TEST_AABB_H_
#define _TEST_AABB_H_

class TestAABB
{
public:
	static void RunTests();

	static void TestIntersects();
	static void TestToString();
};

#endif // !_TEST_AABB_H_
