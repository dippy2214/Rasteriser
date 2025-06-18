#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"
#include "../Camera.h"
#include "../RenderTarget.h"

class VertexShader : public BaseShader
{
	struct InputParams
	{
		float3 vertex;
		float2 textureCoords;
		float3 normals;
	};
	Camera* cam;
	Transform* transform;
	RenderTarget* renderTarget;

	InputParams output;
public:
	void* RunShader(void* shaderParameters, int dataSize) override;
	void SetShaderParameters(Camera* camera, Transform* trans, RenderTarget* renderer);
};

