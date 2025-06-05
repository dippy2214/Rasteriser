#include "BMPImage.h"
#include "float2.h"

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

	return sideAB == sideBC && sideCA == sideBC;
}

int main()
{
	const int width = 400;
	const int height = 400;
	BMPImage bmp(width, height);
	Color backgroundColor(50.0f, 100.0f, 200.0f, 255.0f);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			bmp.set_pixel(x, y, backgroundColor);
		
		}
	}

	float2 a(0.2f * width, 0.2 * height);
	float2 b(0.7f * width, 0.4f * height);
	float2 c(0.4f * width, 0.8f * height);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (IsInsideTriangle(a, b, c, float2(x, y)))
			{
				bmp.set_pixel(x, y, Color(0, 0, 0, 1));
			}
		}
	}

	bmp.write("img_test2.bmp");

	return 0;
}