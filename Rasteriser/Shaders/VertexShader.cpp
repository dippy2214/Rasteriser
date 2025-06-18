#include "VertexShader.h"

void* VertexShader::RunShader(void* shaderParameters, int dataSize)
{
	InputParams* inputs = (InputParams*)shaderParameters;
	return inputs;
}

//void VertexShader::SetShaderParameters(float3* vertexes, float2* textureCoords, float3* normals)
//{
//	
//	//inputParams.vertexArray = vertexes;
//	inputParams.texCoordArray = textureCoords;
//	inputParams.normalArray = normals;
//
//	
//}
