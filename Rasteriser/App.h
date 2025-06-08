#pragma once
#include <stdint.h>
#include <Windows.h>

#include "FloatTypes.h"
#include "Model.h"
#include "RenderTarget.h"
#include "Maths.h"

class App
{
	//windows application stuff
public:
	void InitApp(int WIDTH, int HEIGHT);
	void ProcessFrame(uint32_t* frameBuffer, HWND hwnd);
	void WriteToBuffer(uint32_t* frameBuffer);

private:
	int width;
	int height;

	//my program stuff
public:
	void Render(Model* model, RenderTarget* renderTarget);
	float2 VertexToScreen(float3 vertex, Transform* transform, float2 numPixels);

private:
	Model* cube;
	RenderTarget* renderer;
};

