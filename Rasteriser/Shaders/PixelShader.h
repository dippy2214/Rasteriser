#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"
#include "../BMPImage.h"

class PixelShader : public BaseShader
{
	#pragma pack(push, 1)
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
	#pragma pack(pop)
	BMPImage* modelTexture;
public:
	~PixelShader();
	char* RunShader(void* shaderParameters, int dataSize) override;
	void SetShaderParameters(BMPImage* texture);

};

