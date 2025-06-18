#include "BaseShaderProgram.h"

void BaseShaderProgram::Render(void* inputParams, int inputSize)
{
	std::vector<ShaderOutputs> outputs;
	ShaderDetails* input = (ShaderDetails*)inputParams;
	for (int i = 0; i < inputSize; i+=3)
	{
		ShaderDetails p1 = *(ShaderDetails*)vertexShader->RunShader(&input[i], sizeof(ShaderDetails));
		ShaderDetails p2 = *(ShaderDetails*)vertexShader->RunShader(&input[i+1], sizeof(ShaderDetails));
		ShaderDetails p3 = *(ShaderDetails*)vertexShader->RunShader(&input[i+2], sizeof(ShaderDetails));
		ShaderDetails rasteriserInput[3];
		rasteriserInput[0] = p1;
		rasteriserInput[1] = p2;
		rasteriserInput[2] = p3;
		
		void* temp = rasteriser->RunShader(&rasteriserInput[0], sizeof(rasteriserInput));
		if (temp == nullptr)
		{
			continue;
		}
		RasteriserOutputs pixelShaderInput = *(RasteriserOutputs*)temp;

		for (int i = 0; i < pixelShaderInput.count; i++)
		{
			outputs.emplace_back(*(ShaderOutputs*)pixelShader->RunShader(&pixelShaderInput.outputs[i], sizeof(outputs)));
		}

	}
	
	for (int i = 0; i < outputs.size(); i++)
	{
		renderer->SetPixel(outputs[i].pixelPosition, outputs[i].pixelColor);
	}
	//void* pixelShaderinput = rasteriser->RunShader(rasteriserInput, inputSize);
	//void* outputResults = pixelShader->RunShader(pixelShaderinput, inputSize);

	//std::pair<float4, float2>* pixelData = static_cast<std::pair<float4, float2>*>(outputResults);
	//renderer->SetPixel(pixelData->second.x, pixelData->second.y, pixelData->first);
}

void BaseShaderProgram::SetShaderParameters(BaseShader* vertex, BaseShader* pixel, RenderTarget* renderTarget, Camera* camera)
{
	renderer = renderTarget;

	vertexShader = vertex;
	rasteriser = new Rasteriser();
	rasteriser->SetShaderParameters(renderTarget, camera);
	pixelShader = pixel;
	
}

