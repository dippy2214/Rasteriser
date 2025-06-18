#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"

#include <vector>

class Rasteriser : public BaseShader
{
	struct InputParams
	{
		float3 vertex;
		float2 textureCoords;
		float3 normals;
	};

	struct OutputParams
	{
		float2 screenCoords;
		float4 color;

	};
public:
	void* RunShader(void* shaderParameters, int dataSize) override;
	//void SetShaderParameters(float3* vertexes, float2* textureCoords, float3* normals);

	InputParams inputParams;
	RenderTarget* renderTarget;
	Camera* cam;
};


