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
#include "Shaders/ShaderProgram.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"


App::App(int w, int h, InputManager* inputManager, uint32_t* frameBuffer)
{
	InitApp(w, h, inputManager, frameBuffer);
}

void App::InitApp(int WIDTH, int HEIGHT, InputManager* inputs, uint32_t* frameBuffer)
{
	width = WIDTH;
	height = HEIGHT;
	inputManager = inputs;

	cube = new Model("Models/evilmonkey.obj", "Textures/evilmonkeytexture.bmp");
	cube->position = float3(0, 0, 15);
	renderer = new RenderTarget(width, height, frameBuffer);
	camera = new Camera();
	//camera.position = float3(0, 0, 0);

	basicShader = new ShaderProgram(new VertexShader(), new PixelShader());
}

void App::ProcessFrame(HWND viewPort, float dt)
{
	renderer->Clear();

	Update(dt);
	HandleInput(dt);
	Render();

	UpdateViewport(viewPort);
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
	float3 camMoveDelta;

	
	if (inputManager->isLeftMouseDown())
	{
		float2 mouseDelta = inputManager->GetMouseDragDelta() / renderer->Size().x * mouseSens;
		camera->AddRotationX(mouseDelta.y);
		camera->AddRotationY(-mouseDelta.x);
		
	}

	if (inputManager->IsKeyPressed('W')) { camMoveDelta += camera->basisVectors.khat; }
	if (inputManager->IsKeyPressed('S')) { camMoveDelta -= camera->basisVectors.khat; }
	if (inputManager->IsKeyPressed('A')) { camMoveDelta += camera->basisVectors.ihat; }
	if (inputManager->IsKeyPressed('D')) { camMoveDelta -= camera->basisVectors.ihat; }
	if (inputManager->IsKeyPressed('E')) { camMoveDelta += camera->basisVectors.jhat; }
	if (inputManager->IsKeyPressed('Q')) { camMoveDelta -= camera->basisVectors.jhat; }
	
	camera->position += camMoveDelta.Normalised() * camera->camSpeed * deltaTime;
}

void App::Render()
{
	//auto startRender = std::chrono::high_resolution_clock::now();
	basicShader->RenderObject(cube->GetShaderDetails(), cube, camera, renderer, cube->GetTexture());
	/*auto endRender = std::chrono::high_resolution_clock::now();
	auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endRender - startRender);
	std::cout << "render time: " << totalDuration.count() << "ms" << std::endl;*/
}
