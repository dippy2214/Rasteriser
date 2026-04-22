#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "FloatTypes.h"
#include "Maths.h"
#include "App.h"
#include "Transform.h"
#include "../Graphics/BMPImage.h"
#include "../Graphics/Model.h"
#include "../Graphics/RenderTarget.h"


App::App(int w, int h, InputManager* inputManager, uint32_t* frameBuffer)
{
	InitApp(w, h, inputManager, frameBuffer);
}

void App::InitApp(int WIDTH, int HEIGHT, InputManager* inputs, uint32_t* frameBuffer)
{
	width = WIDTH;
	height = HEIGHT;
	inputManager = inputs;

	cube = new Model("Assets/Models/evilmonkey.obj", "Assets/Textures/evilmonkeytexture.bmp");
	cube->position = float3(0, 0, 15);
	renderer = new RenderTarget(width, height, frameBuffer);

	camera.position = float3(0, 0, 0);

	//initialise audio engine first
	audioEngine.SetActiveListener(&camera);
	
	audioEngine.LoadSound("loop", "Assets/Audio/Loop.wav");
	audioEngine.LoadSound("stereo", "Assets/Audio/Stereo.wav");
	
	audioSource = audioEngine.CreateAudioSource();
	audioSource->SetPosition(float3(0,0,15));
	audioSource->isLooping.store(true);
	audioSource->isActive.store(true);
	audioSource->soundData = audioEngine.GetSound("stereo");
	Mixer* lowPassMixer = audioEngine.AddMixer("low pass filter");
	lowPassMixer->effects.push_back(std::make_unique<LowPassFilter>(500, saudio_sample_rate()));
	audioSource->mixer = lowPassMixer;

	if (!audioSource->soundData)
	{
		std::cout << "Failed to grab sound from loader!\n";
	}

	//stress test setup. Gain at 0.05 to avoid ridiculous clipping and pained ears
	Mixer* gainMixer = audioEngine.AddMixer("gain");
	gainMixer->effects.push_back(std::make_unique<Gain>(0.05f));

	for (int i = 0; i < 63; i++)
	{
		audioSources[i] = audioEngine.CreateAudioSource();
		audioSources[i]->isLooping.store(false);
		audioSources[i]->isActive.store(false);
		audioSources[i]->mixer = gainMixer;
		audioSources[i]->soundData = audioEngine.GetSound("stereo");
	}
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
		camera.AddRotationX(mouseDelta.y);
		camera.AddRotationY(-mouseDelta.x);
		
	}

	if (inputManager->IsKeyPressed('W')) { camMoveDelta += camera.basisVectors.khat; }
	if (inputManager->IsKeyPressed('S')) { camMoveDelta -= camera.basisVectors.khat; }
	if (inputManager->IsKeyPressed('A')) { camMoveDelta += camera.basisVectors.ihat; }
	if (inputManager->IsKeyPressed('D')) { camMoveDelta -= camera.basisVectors.ihat; }
	if (inputManager->IsKeyPressed('E')) { camMoveDelta += camera.basisVectors.jhat; }
	if (inputManager->IsKeyPressed('Q')) { camMoveDelta -= camera.basisVectors.jhat; }
	
	camera.position += camMoveDelta.Normalised() * camera.camSpeed * deltaTime;

	if (inputManager->IsKeyPressed('F')) {
		for (int i = 0; i < 63; i++)
		{
			audioSources[i]->isActive.store(true);
		}
	}
}

void App::Render()
{
	//auto startRender = std::chrono::high_resolution_clock::now();
	cube->Render(renderer, &camera);
	/*auto endRender = std::chrono::high_resolution_clock::now();
	auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endRender - startRender);
	std::cout << "render time: " << totalDuration.count() << "ms" << std::endl;*/
}
