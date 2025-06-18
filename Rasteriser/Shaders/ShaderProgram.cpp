#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(VertexShader* VS, PixelShader* PS)
{
	vertexShader = VS;
	pixelShader = PS;
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::RenderObject(std::pair<int, ShaderDetails*> shaderDetails, Transform* transform, Camera* cam, RenderTarget* renderTarget, BMPImage* tex)
{
	SetShaderParameters(vertexShader, pixelShader, renderTarget, cam);
	vertexShader->SetShaderParameters(cam, transform, renderTarget);
	pixelShader->SetShaderParameters(tex);

	int i = shaderDetails.first;
	Render(shaderDetails.second, shaderDetails.first);
}
