#pragma once
#include "BaseShader.h"
#include "Rasteriser.h"
#include "../RenderTarget.h"
#include "../FloatTypes.h"
#include "../Model.h"

class BaseShaderProgram
{
public:
	void Render(void* inputParams, int inputSize);

	void SetShaderParameters(BaseShader* vertex, BaseShader* pixel, RenderTarget* renderTarget);
private:
	RenderTarget* renderer;

	BaseShader* vertexShader;
	BaseShader* rasteriser;
	BaseShader* pixelShader;

};

