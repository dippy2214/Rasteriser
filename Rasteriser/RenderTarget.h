#pragma once
#include "FloatTypes.h"

class RenderTarget
{
public:
	RenderTarget(int w, int h, uint32_t* pixelBuffer);
	
	float2 Size() { return float2(width, height); }
	void Clear();

	void SetPixel(int x, int y, Color col);
	Color GetPixel(int x, int y);

	void SetDepth(int x, int y, float depthVal);
	float GetDepth(int x, int y);

	uint32_t* colorBuffer;
private:
	uint32_t alphaMask = 0xFF000000;
	uint32_t redMask = 0x00FF0000;
	uint32_t greenMask = 0x0000FF00;
	uint32_t blueMask = 0x000000FF;
	float* depthBuffer;
	int width;
	int height;
};

