// @file: Mesh.cpp
//
// @brief: Cpp file for Mesh class.

#include "stdafx.h"
#include "Engine/Math/Mesh.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Triangle.h"

bool Mesh::LoadFromObjectFile(const std::string& objFilename)
{
	std::ifstream file(objFilename);
	if (!file.is_open())
		return false;

	std::vector<Vector3> vertices;
	while (!file.eof())
	{
		// Assumption: Line size does not exceed 128
		char line[128];
		file.getline(line, 128);

		std::strstream s;
		s << line;

		char temp;  // temp buffer for the 1st char
		// Lines specifying vertices start with 'v'
		if (line[0] == 'v')
		{
			Vector3 vertex;
			s >> temp >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		// Lines specifying triangle faces start with 'f'
		else if (line[0] == 'f')
		{
			int vNum[3];  // vertex numbers
			s >> temp >> vNum[0] >> vNum[1] >> vNum[2];
			
			// Create the triangle & store it in mesh data
			faces.push_back({ vertices[vNum[0] - 1], vertices[vNum[1] - 1], vertices[vNum[2] - 1] });
		}
	}
}
