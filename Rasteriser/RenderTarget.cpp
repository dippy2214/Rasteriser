#include "RenderTarget.h"

RenderTarget::RenderTarget(int w, int h)
{
	width = w; 
	height = h; 
	colorBuffer = new Color [width * height];
}

void RenderTarget::Clear()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			SetPixel(x, y, Color(0, 0, 0, 255));
		}
	}
}

void RenderTarget::SetPixel(int x, int y, Color col)
{
	colorBuffer[y * width + x] = col;
}

Color RenderTarget::GetPixel(int x, int y)
{
	return colorBuffer[y * width + x];
}
