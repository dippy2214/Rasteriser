#include "VertexShader.h"
#include <iostream>

VertexShader::~VertexShader()
{
}

char* VertexShader::RunShader(void* shaderParameters, int dataSize)
{
	InputParams inputs = *(InputParams*)shaderParameters;
	OutputParams output;
	float2 screenSize = renderTarget->Size();
	output.vertex = cam->VertexToScreen(inputs.vertex, transform, screenSize);
	output.normals = inputs.normals;
	output.textureCoords = inputs.textureCoords;

	char* outBuf = new char[sizeof(OutputParams)];
	memcpy(outBuf, &output, sizeof(OutputParams));

	return outBuf;
}

void VertexShader::SetShaderParameters(Camera* camera, Transform* trans, RenderTarget* renderer)
{
	cam = camera;
	transform = trans;
	renderTarget = renderer;
}

