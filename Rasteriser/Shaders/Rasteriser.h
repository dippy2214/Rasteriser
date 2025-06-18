#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"
#include "../Camera.h"
#include "../RenderTarget.h"

#include <vector>

class Rasteriser : public BaseShader
{
	#pragma pack(push, 1)
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
		std::vector<OutputParams> outputs;
	};
	#pragma pack(pop)
public:
	~Rasteriser();
	char* RunShader(void* shaderParameters, int dataSize) override;
	void SetShaderParameters(RenderTarget* renderer, Camera* camera);
	//RasteriserOutput finalOut;
	RenderTarget* renderTarget;
	Camera* cam;
};


