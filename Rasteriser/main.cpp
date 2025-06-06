#include <string>
#include <iostream>
#include <chrono>

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

	float2 speed(10, 10);
	float2 a(0.2f * width, 0.2 * height);
	float2 b(0.7f * width, 0.4f * height);
	float2 c(0.4f * width, 0.8f * height);


	for (int i = 0; i < 1000; i++)
	{
		auto start = std::chrono::system_clock::now();

		bmp.clear_image();

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (IsInsideTriangle(a, b, c, float2(x, y)))
				{
					bmp.set_pixel(x, y, Color(0, 255, 0, 255));
				}
			}
		}
		
		a = a + speed;
		if (a.x < 0 || a.x > width)
		{
			speed.x *= -1;
			a.x += speed.x;
		}
		if (a.y < 0 || a.y > width)
		{
			speed.y *= -1;
			a.y += speed.y;
		}

		b = b + speed;
		if (b.x < 0 || b.x > width)
		{
			speed.x *= -1;
			b.x += speed.x;
		}
		if (b.y < 0 || b.y > width)
		{
			speed.y *= -1;
			b.y += speed.y;
		}

		c = c + speed;
		if (c.x < 0 || c.x > width)
		{
			speed.x *= -1;
			c.x += speed.x;
		}
		if (c.y < 0 || c.y > width)
		{
			speed.y *= -1;
			c.y += speed.y;
		}



		std::string num = std::to_string(i);
		std::string fileName = "AnimFrames/img" + num + ".bmp";
		bmp.write(fileName.c_str());
		auto end = std::chrono::system_clock::now();
		std::cout << "Rendered frame in " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms!\n";
	}

	std::cout << "Finished rendering frames\n";
	return 0;
}