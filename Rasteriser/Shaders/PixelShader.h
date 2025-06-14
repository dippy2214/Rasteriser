#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"
#include "../BMPImage.h"

class PixelShader : public BaseShader
{
	struct InputParams
	{
		float2 screenCoords;
		float2 textureCoords;
		float3 normals;
		//float depth;
	};
	struct OutputParams
	{
		float2 pixelPosition;
		float4 pixelColor;
	};
	OutputParams outputs;

	BMPImage* modelTexture;
public:
	void* RunShader(void* shaderParameters, int dataSize) override;
	void SetShaderParameters(BMPImage* texture);

};

