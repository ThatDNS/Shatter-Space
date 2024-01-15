// @file: Random.h
//
// @brief: Header file for Random class containing basic functionality of generating random numbers.

#pragma once
#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <random>

class Random
{
	std::mt19937 generator;
	std::uniform_real_distribution<float> distribution;

	Random() : generator(std::random_device{}()), distribution(0.0f, 1.0f) {}
	~Random() = default;
	inline explicit Random(Random const&) = delete;
	inline Random& operator=(Random const&) = delete;

public:
	inline static Random& Get() {
		static Random instance;
		return instance;
	}

	// Returns a floating point number between [0, 1].
	float Float()
	{
		return distribution(generator);
	}
};

#endif // !_RANDOM_H_
