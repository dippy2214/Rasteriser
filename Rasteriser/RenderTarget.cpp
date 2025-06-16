#include "RenderTarget.h"
#include "Maths.h"

RenderTarget::RenderTarget(int w, int h, uint32_t* renderTarget)
{
	width = w; 
	height = h; 
	colorBuffer = renderTarget;
	depthBuffer = new float[width * height];
}

RenderTarget::~RenderTarget()
{
	delete depthBuffer;
}

void RenderTarget::Clear()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			SetPixel(x, y, backgroundColor);
			SetDepth(x, y, 1000);
		}
	}
}

void RenderTarget::SetPixel(int x, int y, float4 col)
{
	int red = Maths::Clamp(static_cast<int>(col.x*255), 0, 255);
	int green = Maths::Clamp(static_cast<int>(col.y*255), 0, 255);
	int blue = Maths::Clamp(static_cast<int>(col.z*255), 0, 255);
	int alpha = Maths::Clamp(static_cast<int>(col.w*255), 0, 255);
	uint32_t intCol = alpha << 24 | red << 16 | green << 8 | blue;
	colorBuffer[y * width + x] = intCol;
}

float4 RenderTarget::GetPixel(int x, int y)
{
	Color pixelColor;
	pixelColor.r = static_cast<uint32_t>(colorBuffer[y * width + x] && redMask);
	pixelColor.g = static_cast<uint32_t>(colorBuffer[y * width + x] && greenMask);
	pixelColor.b = static_cast<uint32_t>(colorBuffer[y * width + x] && blueMask);
	pixelColor.a = static_cast<uint32_t>(colorBuffer[y * width + x] && alphaMask);
	float4 out(pixelColor);
	return out;
}

void RenderTarget::SetDepth(int x, int y, float depthVal)
{
	depthBuffer[y * width + x] = depthVal;
}

float RenderTarget::GetDepth(int x, int y)
{
	return depthBuffer[y * width + x];
}
