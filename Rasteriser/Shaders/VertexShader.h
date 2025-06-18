#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"

class VertexShader : public BaseShader
{
	struct InputParams
	{
		float3 vertex;
		float2 textureCoords;
		float3 normals;
	};
public:
	void* RunShader(void* shaderParameters, int dataSize) override;
	//void SetShaderParameters(float3* vertexes, float2* textureCoords, float3* normals);

	//InputParams inputParams;
};

