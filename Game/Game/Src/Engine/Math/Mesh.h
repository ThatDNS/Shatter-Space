// @file: Mesh.h
//
// @brief: Header file for Mesh class.

#pragma once
#ifndef _MESH_H_
#define _MESH_H_

#include "Engine/Math/Triangle.h"

class Mesh
{
public:
	std::vector<Triangle> faces;

	bool LoadFromObjectFile(const std::string&);
};

#endif // !_MESH_H_
