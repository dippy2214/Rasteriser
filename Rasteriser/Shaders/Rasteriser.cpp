#include "Rasteriser.h"
#include "../Maths.h"
#include "../Camera.h"
#include "../RenderTarget.h"

void* Rasteriser::RunShader(void* shaderParameters, int dataSize)
{
	InputParams* inputs = (InputParams*)shaderParameters;
	//auto startTri = std::chrono::high_resolution_clock::now();
	float2 screenSize = renderTarget->Size();
	float3 a = cam->VertexToScreen(inputs[0].vertex, this, screenSize);
	float3 b = cam->VertexToScreen(inputs[1].vertex, this, screenSize);
	float3 c = cam->VertexToScreen(inputs[2].vertex, this, screenSize);

	//std::cout << a.ToString() << " : " << b.ToString() << " : " << c.ToString() << std::endl;

	if (a.z <= 0 || b.z <= 0 || c.z <= 0) { return; }

	float maxX = Maths::LargestOfThree(a.x, b.x, c.x);
	float minX = Maths::SmallestOfThree(a.x, b.x, c.x);
	float maxY = Maths::LargestOfThree(a.y, b.y, c.y);
	float minY = Maths::SmallestOfThree(a.y, b.y, c.y);

	int blockStartX = Maths::Clamp((int)minX, 0, screenSize.x - 1);
	int blockEndX = Maths::Clamp((int)ceilf(maxX), 0, screenSize.x - 1);
	int blockStartY = Maths::Clamp((int)minY, 0, screenSize.y - 1);
	int blockEndY = Maths::Clamp((int)ceilf(maxY), 0, screenSize.y - 1);

	std::vector<OutputParams> outputs;

	for (int y = blockStartY; y <= blockEndY; y++)
	{
		for (int x = blockStartX; x <= blockEndX; x++)
		{
			float2 p(x, y);
			float3 triWeights;

			if (!Maths::IsInsideTriangle(a.xy(), b.xy(), c.xy(), p, &triWeights)) { continue; }

			float3 depths(a.z, b.z, c.z);
			float depth = 1 / ((1.0f / depths).Dot(triWeights));

			if (depth > renderTarget->GetDepth(x, y)) { continue; }
			

		}
	}

	return shaderParameters;
}

//void Rasteriser::SetShaderParameters(float3* vertexes, float2* textureCoords, float3* normals)
//{
//	
//	inputParams.vertexArray = vertexes;
//	inputParams.texCoordArray = textureCoords;
//	inputParams.normalArray = normals;
//
//	
//}