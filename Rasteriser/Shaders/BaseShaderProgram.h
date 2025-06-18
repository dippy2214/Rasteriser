#pragma once
#include "BaseShader.h"
#include "Rasteriser.h"
#include "../RenderTarget.h"
#include "../FloatTypes.h"
#include "../Model.h"
#include <vector>
#include <thread>
#include <mutex>

class BaseShaderProgram
{
	#pragma pack(push, 1)
	struct RasteriserOutputParams
	{
		float2 screenCoords;
		float2 textureCoords;
		float3 normals;
	};
	struct RasteriserOutputs
	{
		int count;
		RasteriserOutputParams* outputs;
	};

	struct ShaderOutputs
	{
		float2 pixelPosition;
		float4 pixelColor;
	};
	#pragma pack(pop)
public:
	void Render(void* inputParams, int inputSize);
	void ThreadWorker(int start, int end, std::vector<ShaderOutputs>* outputs, ShaderDetails* input); 
	void SetShaderParameters(BaseShader* vertex, BaseShader* pixel, RenderTarget* renderTarget, Camera* camera);
private:
	RenderTarget* renderer;

	BaseShader* vertexShader;
	Rasteriser rasteriser;
	BaseShader* pixelShader;

	std::mutex mtx;
};

