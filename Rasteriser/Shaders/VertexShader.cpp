#include "VertexShader.h"
#include <iostream>

void* VertexShader::RunShader(void* shaderParameters, int dataSize)
{
	InputParams inputs = *(InputParams*)shaderParameters;
	float2 screenSize = renderTarget->Size();
	output.vertex = cam->VertexToScreen(inputs.vertex, transform, screenSize);
	output.normals = inputs.normals;
	output.textureCoords = inputs.textureCoords;
	return &output;
}

void VertexShader::SetShaderParameters(Camera* camera, Transform* trans, RenderTarget* renderer)
{
	cam = camera;
	transform = trans;
	renderTarget = renderer;
}

