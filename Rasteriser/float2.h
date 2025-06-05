#pragma once
class float2
{
public:
	float2() {}
	float2(float x1, float y1) { x = x1; y = y1; }

	float2 operator+(float2 const& obj)
	{
		float2 result(x, y);
		result.x += obj.x;
		result.y += obj.y;
		return result;
	}
	float2 operator-(float2 const& obj)
	{
		float2 result(x, y);
		result.x -= obj.x;
		result.y -= obj.y;
		return result;
	}

	float Dot(float2 b) { return x* b.x + y * b.y; }
	float2 Perpendicular() { return float2(y, -x); }


	float x = 0;
	float y = 0;
};

