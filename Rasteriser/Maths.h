#pragma once
#include "FloatTypes.h"

class Maths
{
public:
	static bool IsOnRightSideOfLine(float2 a, float2 b, float2 p);
	static bool IsInsideTriangle(float2 a, float2 b, float2 c, float2 p);

	static float LargestOfThree(float a, float b, float c);
	static float SmallestOfThree(float a, float b, float c);
	
	static int Clamp(int x, int min, int max);

};

