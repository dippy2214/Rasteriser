#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"
#include "../Camera.h"
#include "../RenderTarget.h"

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
		float2 textureCoords;
		float3 normals;
	};

	struct RasteriserOutput
	{
		int count;
		OutputParams* outputs;
	};
public:
	void* RunShader(void* shaderParameters, int dataSize) override;
	void SetShaderParameters(RenderTarget* renderer, Camera* camera);
	std::vector<OutputParams> outputs;
	RasteriserOutput finalOut;
	RenderTarget* renderTarget;
	Camera* cam;
};


