#pragma once
#include <string>
#include <stdint.h>

class Color
{
public:
	Color() { r = 0; b = 0; g = 0; a = 255; }
	Color(int r1, int g1, int b1, int a1) { r = r1; g = g1; b = b1; a = a1; }
	//Color(Color x1) { r = x1.r; g = x1.g; b = x1.b; a = x1.a; }

	Color operator*(Color const& x1)
	{
		Color result(r, g, b, a);
		result.r *= x1.r;
		result.g *= x1.g;
		result.b *= x1.b;
		result.a *= x1.a;
		return result;
	}
	Color operator*(float const& x1)
	{
		Color result(r, g, b, a);
		result.r *= x1;
		result.g *= x1;
		result.b *= x1;
		result.a *= x1;
		return result;
	}
	Color operator/(float const& x1)
	{
		Color result(r, g, b, a);
		result.r /= x1;
		result.g /= x1;
		result.b /= x1;
		result.a /= x1;
		return result;
	}

	std::string ToString();
	int32_t ToInt() { return a << 24 | r << 16 | g << 8 | b; }

	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;
};