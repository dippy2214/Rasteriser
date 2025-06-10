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
#include "Transform.h"


App::App(int w, int h, InputManager* inputManager)
{
	InitApp(w, h, inputManager);
}

void App::InitApp(int WIDTH, int HEIGHT, InputManager* inputs)
{
	width = WIDTH;
	height = HEIGHT;
	inputManager = inputs;

	cube = new Model("Models/evilmonkey.obj");
	cube->position = float3(0, 0, 15);
	renderer = new RenderTarget(width, height);

	camera.position = float3(0, 2, 0);
}

void App::ProcessFrame(uint32_t* frameBuffer, HWND hwnd, float dt)
{
	renderer->Clear();

	Update(dt);
	HandleInput(dt);
	Render();

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


//float3 App::VertexToScreen(float3 vertex, Transform* transform, float2 numPixels, float fov)
//{
//	float3 worldPoint = transform->ToWorldPoint(vertex);
//	float3 viewPoint = camera.ToLocalPoint(worldPoint);
//
//	float screenHeight_world = tan(fov / 2) * 2;
//	float pixelsPerWorldUnit = numPixels.y / screenHeight_world / viewPoint.z;
//
//	float2 pixelOffset = float2(viewPoint.x, viewPoint.y) * pixelsPerWorldUnit;
//	float2 screenPoint = numPixels / 2 + pixelOffset;
//
//	return float3(screenPoint.x, screenPoint.y, viewPoint.z);
//}



void App::Update(float deltaTime)
{
	//cube->Yaw += 3*deltaTime;
	//cube->Pitch += 2*deltaTime;

}

void App::HandleInput(float deltaTime)
{
	Transform::BasisVectors camVecs = camera.GetBasisVectors();
	float3 camMoveDelta;

	
	if (inputManager->isLeftMouseDown())
	{
		float2 mouseDelta = inputManager->GetMouseDragDelta() / renderer->Size().x * mouseSens;
		camera.Pitch += mouseDelta.y;
		camera.Yaw -= mouseDelta.x;
	}

	if (inputManager->IsKeyPressed('W')) { camMoveDelta += camVecs.khat; }
	if (inputManager->IsKeyPressed('S')) { camMoveDelta -= camVecs.khat; }
	if (inputManager->IsKeyPressed('A')) { camMoveDelta += camVecs.ihat; }
	if (inputManager->IsKeyPressed('D')) { camMoveDelta -= camVecs.ihat; }
	if (inputManager->IsKeyPressed('E')) { camMoveDelta += camVecs.jhat; }
	if (inputManager->IsKeyPressed('Q')) { camMoveDelta -= camVecs.jhat; }
	


	camera.position += camMoveDelta.Normalised() * camera.camSpeed * deltaTime;
}

void App::Render()
{
	cube->Render(renderer, &camera);
}

//void App::Render(Model* model, RenderTarget* renderTarget)
//{
//	for (int i = 0; i < model->points.size(); i += 3)
//	{
//		float2 screenSize = renderTarget->Size();
//		float3 a = VertexToScreen(model->points[i + 0], model, renderTarget->Size(), 30.0f);
//		float3 b = VertexToScreen(model->points[i + 1], model, renderTarget->Size(), 30.0f);
//		float3 c = VertexToScreen(model->points[i + 2], model, renderTarget->Size(), 30.0f);
//
//		if (a.z <= 0 || b.z <= 0 || c.z <= 0) { continue; }
//		 
//		float maxX = Maths::LargestOfThree(a.x, b.x, c.x);
//		float minX = Maths::SmallestOfThree(a.x, b.x, c.x);
//		float maxY = Maths::LargestOfThree(a.y, b.y, c.y);
//		float minY = Maths::SmallestOfThree(a.y, b.y, c.y);
//
//		int blockStartX = Maths::Clamp((int)minX, 0, screenSize.x - 1);
//		int blockEndX = Maths::Clamp((int)ceilf(maxX), 0, screenSize.x - 1);
//		int blockStartY = Maths::Clamp((int)minY, 0, screenSize.y - 1);
//		int blockEndY = Maths::Clamp((int)ceilf(maxY), 0, screenSize.y - 1);
//
//		for (int y = blockStartY; y <= blockEndY; y++)
//		{
//			for (int x = blockStartX; x <= blockEndX; x++)
//			{
//				float2 p(x, y);
//				float3 triWeights;
//
//				if (!Maths::IsInsideTriangle(a.xy(), b.xy(), c.xy(), p, &triWeights)) { continue; }
//					
//				float3 depths(a.z, b.z, c.z);
//				float depth = 1/((1.0f/depths).Dot(triWeights));
//
//				if (depth > renderTarget->GetDepth(x, y)) { continue; }
//				
//				renderTarget->SetPixel(x, y, model->triColours[i / 3]);
//				renderTarget->SetDepth(x, y, depth);
//			}
//		}
//	}
//}

