// @file: TestBVH.h
//
// @brief: Header file for TestBVH class containing unit tests for BVH (Bounding Volume Hierarchy) class.

#pragma once
#ifndef _TEST_BVH_H_
#define _TEST_BVH_H_

class BVH;
class BoxCollider;

class TestBVH
{
	static void TestBuildTree(BVH*, std::vector<BoxCollider*>&);
	static void TestDestroy(BVH*);
	static void TestCheckCollisions(BVH*);

public:
	static void RunTests();
};

#endif // !_TEST_BVH_H_
