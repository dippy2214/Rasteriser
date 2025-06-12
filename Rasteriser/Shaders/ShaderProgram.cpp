#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(VertexShader* VS, PixelShader* PS)
{
	vertexShader = VS;
	pixelShader = PS;
}

void ShaderProgram::RenderObject(std::pair<int, ShaderDetails*> shaderDetails, RenderTarget* renderTarget)
{
	SetShaderParameters(vertexShader, pixelShader, renderTarget);

	//int i = shaderDetails.first;
	Render(shaderDetails.second, shaderDetails.first);
}
