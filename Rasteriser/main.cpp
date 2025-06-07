#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "BMPImage.h"
#include "FloatTypes.h"
#include "Model.h"
#include "RenderTarget.h"

bool IsOnRightSideOfLine(float2 a, float2 b, float2 p)
{
	float2 ap = p - a;
	float2 abPerp = (b - a).Perpendicular();
	return ap.Dot(abPerp) >= 0;
}
bool IsInsideTriangle(float2 a, float2 b, float2 c, float2 p)
{
	bool sideAB = IsOnRightSideOfLine(a, b, p);
	bool sideBC = IsOnRightSideOfLine(b, c, p);
	bool sideCA = IsOnRightSideOfLine(c, a, p);

	return sideAB && sideBC && sideCA;
}

float LargestOfThree(float a, float b, float c)
{
	if (a >= b)
	{
		if (a >= c)
		{
			return a;
		}
		else
		{
			return c;
		}
	}
	else
	{
		if (b >= c)
		{
			return b;
		}
		else
		{
			return c;
		}
	}
}
float SmallestOfThree(float a, float b, float c)
{
	if (a <= b)
	{
		if (a <= c)
		{
			return a;
		}
		else
		{
			return c;
		}
	}
	else
	{
		if (b <= c)
		{
			return b;
		}
		else
		{
			return c;
		}
	}
}

int Clamp(int x, int min, int max)
{
	if (x < min)
	{
		return min;
	}
	else if (x > max)
	{
		return max;
	}
	else
	{
		return x;
	}
}

float2 VertexToScreen(float3 vertex, Transform* transform, float2 numPixels)
{
	float3 worldPoint = transform->ToWorldPoint(vertex);

	float screenHeight_world = 5.0f;
	float pixelsPerWorldUnit = numPixels.y / screenHeight_world;

	float2 pixelOffset = float2(worldPoint.x, worldPoint.y) * pixelsPerWorldUnit;

	return (numPixels / 2.0f) + pixelOffset;
}



void Render(Model* model, RenderTarget* renderTarget)
{
	//std::cout << renderTarget->Size().ToString() << "\n";
	for (int i = 0; i < model->points.size(); i+=3)
	{
		//std::cout << model->points[i + 0].ToString() << " :: " << model->points[i + 1].ToString() << " :: " << model->points[i + 2].ToString() << "\n";

		float2 a = VertexToScreen(model->points[i + 0], model, renderTarget->Size());
		float2 b = VertexToScreen(model->points[i + 1], model, renderTarget->Size());
		float2 c = VertexToScreen(model->points[i + 2], model, renderTarget->Size());
		 
		//std::cout << a.ToString() << " :: " << b.ToString() << " :: " << c.ToString() << "\n";

		float maxX = LargestOfThree(a.x, b.x, c.x);
		float minX = SmallestOfThree(a.x, b.x, c.x);
		float maxY = LargestOfThree(a.y, b.y, c.y);
		float minY = SmallestOfThree(a.y, b.y, c.y);

		int blockStartX = Clamp((int)minX, 0, renderTarget->width - 1);
		int blockEndX = Clamp((int)ceilf(maxX), 0, renderTarget->width - 1);
		int blockStartY = Clamp((int)minY, 0, renderTarget->height - 1);
		int blockEndY = Clamp((int)ceilf(maxY), 0, renderTarget->height - 1);


		
		for (int y = blockStartY; y <= blockEndY; y++)
		{
			for (int x = blockStartX; x <= blockEndX; x++)
			{
				if (IsInsideTriangle(a, b, c, float2(x, y)))
				{
					renderTarget->SetPixel(x, y, model->triColours[i / 3]);
				}
				
			}
		}
	}
}

int main()
{
	const int width = 800;
	const int height = 600;
	
	Model cube("Models/cube.obj");
	RenderTarget renderer(width, height);
	BMPImage bmp(width, height);

	for (int i = 0; i < 500; i++)
	{
		bmp.clear_image();
		renderer.Clear();

		Render(&cube, &renderer);

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				bmp.set_pixel(x, y, renderer.GetPixel(x, y));
			}
		}
		cube.Yaw += 0.1;
		std::string fileName = "AnimFrames/cube" + std::to_string(i) + ".bmp";
		bmp.write(fileName.c_str());
	}
	return 0;
}