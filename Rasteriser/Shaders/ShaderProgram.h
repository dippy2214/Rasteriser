#pragma once
#include "BaseShaderProgram.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "../RenderTarget.h"
#include "../Model.h"

class ShaderProgram : public BaseShaderProgram
{
public:
	ShaderProgram(VertexShader* VS, PixelShader* PS);
	void RenderObject(std::pair<int, ShaderDetails*> shaderDetails, Transform* transform, Camera* cam, RenderTarget* renderTarget, BMPImage* tex);

	VertexShader* vertexShader;
	PixelShader* pixelShader;
};

