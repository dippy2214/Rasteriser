#include "PixelShader.h"

void* PixelShader::RunShader(void* shaderParameters, int dataSize)
{
	std::pair<float4, float2> result;
	result = { float4(1, 0, 0, 1), float2(100, 100) };
	return static_cast<void*>(&result);
}

//void PixelShader::SetShaderParameters(float3* vertexes, float2* textureCoords, float3* normals)
//{
//	
//	inputParams.vertexArray = vertexes;
//	inputParams.texCoordArray = textureCoords;
//	inputParams.normalArray = normals;
//
//	
//}