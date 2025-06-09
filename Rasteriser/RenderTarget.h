#pragma once
#include "FloatTypes.h"

class RenderTarget
{
public:
	RenderTarget(int w, int h);
	
	float2 Size() { return float2(width, height); }
	void Clear();

	void SetPixel(int x, int y, Color col);
	Color GetPixel(int x, int y);

	void SetDepth(int x, int y, float depthVal);
	float GetDepth(int x, int y);

	Color* colorBuffer;
private:
	
	float* depthBuffer;
	int width;
	int height;
};

