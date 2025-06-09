#pragma once
#include "FloatTypes.h"

class Maths
{
public:
	static float SignedTriangleArea(float2 a, float2 b, float2 p);
	static bool IsInsideTriangle(float2 a, float2 b, float2 c, float2 p, float3* outputWeights);

	static float LargestOfThree(float a, float b, float c);
	static float SmallestOfThree(float a, float b, float c);
	
	static int Clamp(int x, int min, int max);

};

