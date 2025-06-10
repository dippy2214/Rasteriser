#pragma once
#include <string>
#include <vector>

#include "FloatTypes.h"
#include "Transform.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "BMPImage.h"

struct PointData
{
	int vertexIndex;
	int textureCoordIndex;
	int normalIndex;
};
class Model : public Transform
{
	
public: 
	
	Model(std::string fileName);
	Model(std::string fileName, std::string textureFileName);
	std::vector<float3> LoadObjFile(std::string fileName);
	void Render(RenderTarget* renderTarget, Camera* cam);

	bool hasTexture;
private:
	float3 ParsePoints(std::string str);
	float2 ParseTextureCoords(std::string str);
	std::vector<PointData> ParseFaces(std::string str);
	
	BMPImage* modelTexure;

	std::vector<float3> points;
	std::vector<float2> textureCoords;
	std::vector<Color> triColours;
};

