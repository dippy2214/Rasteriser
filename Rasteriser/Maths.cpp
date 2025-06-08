#include "Maths.h"


bool Maths::IsOnRightSideOfLine(float2 a, float2 b, float2 p)
{
	float2 ap = p - a;
	float2 abPerp = (b - a).Perpendicular();
	return ap.Dot(abPerp) >= 0;
}
bool Maths::IsInsideTriangle(float2 a, float2 b, float2 c, float2 p)
{
	bool sideAB = IsOnRightSideOfLine(a, b, p);
	bool sideBC = IsOnRightSideOfLine(b, c, p);
	bool sideCA = IsOnRightSideOfLine(c, a, p);
	return sideAB && sideBC && sideCA;
}

float Maths::LargestOfThree(float a, float b, float c)
{
	if (a >= b)
	{
		if (a >= c) { return a; }
		else { return c; }
	}
	else {
		if (b >= c) { return b; }
		else { return c; }
	}
}
float Maths::SmallestOfThree(float a, float b, float c)
{
	if (a <= b)
	{
		if (a <= c) { return a; }
		else { return c; }
	}
	else {
		if (b <= c) { return b; }
		else { return c; }
	}
}

int Maths::Clamp(int x, int min, int max)
{
	if (x < min) { return min; }
	else if (x > max) { return max; }
	else { return x; }
}