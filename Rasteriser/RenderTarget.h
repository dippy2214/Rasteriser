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

	Color* colorBuffer;
	int width;
	int height;
};

