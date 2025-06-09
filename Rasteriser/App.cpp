#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "BMPImage.h"
#include "FloatTypes.h"
#include "Model.h"
#include "RenderTarget.h"
#include "Maths.h"
#include "App.h"


void App::InitApp(int WIDTH, int HEIGHT)
{
	width = WIDTH;
	height = HEIGHT;

	cube = new Model("Models/cube.obj");
	cube->position = float3(0, 0, 4);
	renderer = new RenderTarget(width, height);
}

void App::ProcessFrame(uint32_t* frameBuffer, HWND hwnd)
{
	renderer->Clear();
	
	Render(cube, renderer);

	WriteToBuffer(frameBuffer);

	InvalidateRect(hwnd, NULL, true);
	UpdateWindow(hwnd);
}

void App::WriteToBuffer(uint32_t* frameBuffer)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			frameBuffer[y * width + x] = renderer->GetPixel(x, y).ToInt();
		}
	}
}


float2 App::VertexToScreen(float3 vertex, Transform* transform, float2 numPixels, float fov)
{
	float3 worldPoint = transform->ToWorldPoint(vertex);

	float3 camPoint = worldPoint;

	if (camPoint.z <= 0.0f)
	{
		return float2(-1000, -1000);
	}

	float viewX = camPoint.x / -camPoint.z;
	float viewY = camPoint.y / -camPoint.z;

	float tanHalfFOV = tanf(fov * 0.5);
	float aspectRatio = numPixels.x / numPixels.y;

	float ndcX = viewX / (tanHalfFOV * aspectRatio);
	float ndcY = viewY / tanHalfFOV;

	//float screenHeight_world = tanf(fov/2.0f)*2.0f;
	//float pixelsPerWorldUnit = numPixels.y / screenHeight_world / worldPoint.z;

	//float2 pixelOffset = float2(worldPoint.x, worldPoint.y) * pixelsPerWorldUnit;
	//return (numPixels / 2.0f) + pixelOffset;

	float2 screenPos;
	screenPos.x = (ndcX + 1.0f) * 0.5f * numPixels.x;
	screenPos.y = (ndcY + 1.0f) * 0.5f * numPixels.y;

	return screenPos;
}



void App::Render(Model* model, RenderTarget* renderTarget)
{
	for (int i = 0; i < model->points.size(); i += 3)
	{
		float2 a = VertexToScreen(model->points[i + 0], model, renderTarget->Size(), 30.0f);
		float2 b = VertexToScreen(model->points[i + 1], model, renderTarget->Size(), 30.0f);
		float2 c = VertexToScreen(model->points[i + 2], model, renderTarget->Size(), 30.0f);
		 
		float maxX = Maths::LargestOfThree(a.x, b.x, c.x);
		float minX = Maths::SmallestOfThree(a.x, b.x, c.x);
		float maxY = Maths::LargestOfThree(a.y, b.y, c.y);
		float minY = Maths::SmallestOfThree(a.y, b.y, c.y);

		int blockStartX = Maths::Clamp((int)minX, 0, renderTarget->width - 1);
		int blockEndX = Maths::Clamp((int)ceilf(maxX), 0, renderTarget->width - 1);
		int blockStartY = Maths::Clamp((int)minY, 0, renderTarget->height - 1);
		int blockEndY = Maths::Clamp((int)ceilf(maxY), 0, renderTarget->height - 1);


		
		for (int y = blockStartY; y <= blockEndY; y++)
		{
			for (int x = blockStartX; x <= blockEndX; x++)
			{
				if (Maths::IsInsideTriangle(a, b, c, float2(x, y)))
				{
					renderTarget->SetPixel(x, y, model->triColours[i / 3]);
				}
			}
		}
	}
	cube->Yaw += 0.03;
	cube->Pitch += 0.02;
	//std::cout << cube->Yaw << "\n";
}

