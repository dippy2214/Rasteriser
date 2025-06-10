#pragma once
#include <stdint.h>
#include <Windows.h>

#include "FloatTypes.h"
#include "Model.h"
#include "RenderTarget.h"
#include "Maths.h"
#include "Camera.h"
#include "InputManager.h"

class App
{
	//windows application stuff
public:
	App(int w, int h, InputManager* inputs);
	void InitApp(int WIDTH, int HEIGHT, InputManager* inputs);
	void ProcessFrame(uint32_t* frameBuffer, HWND viewPort, float dt);
	void WriteToBuffer(uint32_t* frameBuffer);
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
	/*void Render(Model* model, RenderTarget* renderTarget);
	float3 VertexToScreen(float3 vertex, Transform* transform, float2 numPixels, float fov);*/

private:
	InputManager* inputManager;
	Camera camera;
	Model* cube;
	RenderTarget* renderer;
};

