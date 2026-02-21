#include "Maths.h"


float Maths::SignedTriangleArea(float2 a, float2 b, float2 c)
{
	float2 ac = c - a;
	float2 abPerp = (b - a).Perpendicular();
	return ac.Dot(abPerp) / 2.0f;
}

bool Maths::IsInsideTriangle(float2 a, float2 b, float2 c, float2 p, float3* outputWeights)
{
	float areaABP = SignedTriangleArea(a, b, p);
	float areaBCP = SignedTriangleArea(b, c, p);
	float areaCAP = SignedTriangleArea(c, a, p);
	bool inTri = (areaABP > 0 && areaBCP > 0 && areaCAP > 0);

	float totalArea = (areaABP + areaBCP + areaCAP);
	float invAreaSum = 1 / totalArea;
	outputWeights->x = areaBCP * invAreaSum;
	outputWeights->y = areaCAP * invAreaSum;
	outputWeights->z = areaABP * invAreaSum;

	return inTri && totalArea > 0;
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