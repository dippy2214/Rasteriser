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

void App::ProcessFrame(uint32_t* frameBuffer, HWND viewPort, float dt)
{
	renderer->Clear();

	Update(dt);
	HandleInput(dt);
	Render();

	WriteToBuffer(frameBuffer);
	UpdateViewport(viewPort);
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

void App::UpdateViewport(HWND viewPort)
{
	InvalidateRect(viewPort, NULL, true);
	UpdateWindow(viewPort);
}

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
