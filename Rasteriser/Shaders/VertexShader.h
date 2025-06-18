#pragma once
#include "BaseShader.h"
#include "../FloatTypes.h"
#include "../Camera.h"
#include "../RenderTarget.h"

class VertexShader : public BaseShader
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
		float3 vertex;
		float2 textureCoords;
		float3 normals;
	};
	#pragma pack(pop)
	Camera* cam;
	Transform* transform;
	RenderTarget* renderTarget;

	//InputParams output;
public:
	~VertexShader();
	char* RunShader(void* shaderParameters, int dataSize) override;
	void SetShaderParameters(Camera* camera, Transform* trans, RenderTarget* renderer);
};

