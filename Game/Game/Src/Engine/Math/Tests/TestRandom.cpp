// @file: TestRandom.cpp
//
// @brief: Cpp file for TestRandom class containing unit tests for Random class.

#include "stdafx.h"
#include "TestRandom.h"
#include "Engine/Math/Random.h"
#include "Engine/Core/Logger.h"

void TestRandom::RunTests()
{
	TestFloat();
	Logger::Get().Log("[UNITTEST] Random - All tests passed!");
}

void TestRandom::TestFloat()
{
	float num1 = Random::Get().Float();
	float num2 = Random::Get().Float();
	assert(num1 != num2);
}
