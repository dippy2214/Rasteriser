#include "BaseShaderProgram.h"

void BaseShaderProgram::Render(void* inputParams, int inputSize)
{
	ShaderDetails* input = (ShaderDetails*)inputParams;
	for (int i = 0; i < inputSize; i+=3)
	{
		ShaderDetails rasteriserInput[3] = { input[i], input[i + 1], input[i + 2] };
		ShaderDetails* rasteriserInput = (ShaderDetails*)vertexShader->RunShader(rasteriserInput, inputSize);

	}
	
	void* pixelShaderinput = rasteriser->RunShader(rasteriserInput, inputSize);
	void* outputResults = pixelShader->RunShader(pixelShaderinput, inputSize);

	std::pair<float4, float2>* pixelData = static_cast<std::pair<float4, float2>*>(outputResults);
	renderer->SetPixel(pixelData->second.x, pixelData->second.y, pixelData->first);
}

void BaseShaderProgram::SetShaderParameters(BaseShader* vertex, BaseShader* pixel, RenderTarget* renderTarget)
{
	renderer = renderTarget;

	vertexShader = vertex;
	rasteriser = new Rasteriser();
	pixelShader = pixel;
	
}

