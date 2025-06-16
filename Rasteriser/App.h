#pragma once
#include <stdint.h>
#include <Windows.h>

#include "FloatTypes.h"
#include "Model.h"
#include "RenderTarget.h"
#include "Maths.h"
#include "Camera.h"
#include "InputManager.h"
#include "Shaders/ShaderProgram.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"

class App
{
	//windows application stuff
public:
	App(int w, int h, InputManager* inputs, uint32_t* frameBuffer);
	~App();
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
	InputManager* inputManager;
	Camera* camera;
	Model* cube;
	RenderTarget* renderer;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	ShaderProgram* basicShader;
};

