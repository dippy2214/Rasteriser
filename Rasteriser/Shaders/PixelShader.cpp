#include "PixelShader.h"

void* PixelShader::RunShader(void* shaderParameters, int dataSize)
{
	InputParams* inputs = (InputParams*)shaderParameters;
	outputs.pixelPosition = inputs->screenCoords;
	float4 textureColor = modelTexture->get_pixel(inputs->textureCoords.x, inputs->textureCoords.y);
	float lightIntensity = std::max(0.0f, inputs->normals.Dot(float3(0, 1, 1)));
	float4 lightingColor = float4(1, 1, 1, 1) * lightIntensity;

	//std::cout << "light intensity: " << lightIntensity << " : lighting Color: " << lightingColor.ToString() << std::endl;

	float4 finalColor = lightingColor * textureColor;
	outputs.pixelColor = finalColor;

	
	return &outputs;
}

void PixelShader::SetShaderParameters(BMPImage* texture)
{
	modelTexture = texture;
}
