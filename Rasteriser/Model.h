#pragma once
#include <string>
#include <vector>

#include "FloatTypes.h"
#include "Transform.h"

class Model : public Transform
{
public: 
	Model(std::string fileName);
	float3 ParsePoints(std::string str);
	float2 ParseTextureCoords(std::string str);
	std::vector<int> ParseFaces(std::string str);
	std::vector<float3> LoadObjFile(std::string fileName);


	std::vector<float3> points;
	std::vector<float2> textureCoords;
	std::vector<Color> triColours;
};

