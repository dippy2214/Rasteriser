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

struct ShaderDetails
{
public:
	float3 vertex;
	float2 textureCoord;
	float3 normal;
};

class Model : public Transform
{
public: 	
	Model(std::string fileName);
	Model(std::string fileName, std::string textureFileName);
	std::vector<float3> LoadObjFile(std::string fileName);
	void Render(RenderTarget* renderTarget, Camera* cam);
	std::pair<int, ShaderDetails*> GetShaderDetails();
	BMPImage* GetTexture();

	bool hasTexture;

private:
	float3 ParseVertexOrNormals(std::string str, bool isNormal = false);
	float2 ParseTextureCoords(std::string str);
	std::vector<PointData> ParseFaces(std::string str);
	
	
	BMPImage* modelTexture;
	std::vector<ShaderDetails> shaderDetails;

public:
	std::vector<float3> points;
	std::vector<float2> textureCoords;
	std::vector<float3> normals;
	std::vector<Color> triColours;
};

