#pragma once
#include <stdint.h>
#include <Windows.h>

#include "FloatTypes.h"
#include "InputManager.h"
#include "Maths.h"
#include "../Graphics/Model.h"
#include "../Graphics/RenderTarget.h"
#include "../Graphics/Camera.h"
#include "../Audio/AudioEngine.h"


class App
{
	//windows application stuff
public:
	App(int w, int h, InputManager* inputs, uint32_t* frameBuffer);
	void InitApp(int WIDTH, int HEIGHT, InputManager* inputs, uint32_t* pixelBuffer);
	void ProcessFrame(HWND viewPort, float dt);
	void UpdateViewport(HWND viewPort);

private:
	int width = 0;
	int height = 0;
	float mouseSens = 2;

	//my program stuff
public:
	void Update(float deltaTime);
	void HandleInput(float deltaTime);
	void Render();

private:
	AudioEngine audioEngine;
	InputManager* inputManager;
	Camera camera;
	Model* cube;
	RenderTarget* renderer;
	AudioSource* audioSource;
};

