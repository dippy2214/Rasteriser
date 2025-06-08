#pragma once
#include <string>
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
	float2 operator*(float const& x1)
	{
		float2 result(x, y);
		result.x *= x1;
		result.y *= x1;
		return result;
	}
	float2 operator/(float const& x1)
	{
		float2 result(x, y);
		result.x /= x1;
		result.y /= x1;
		return result;
	}

	std::string ToString()
	{
		std::string output;
		output += std::to_string(x);
		output += ", ";
		output += std::to_string(y);
		return output;
	}

	float Dot(float2 b) { return x* b.x + y * b.y; }
	float2 Perpendicular() { return float2(y, -x); }


	float x = 0;
	float y = 0;
};

class float3
{
public:
	float3() {}
    float3(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }

	float3 operator+(float3 const& obj)
	{
		float3 result(x, y, z);
		result.x += obj.x;
		result.y += obj.y;
		result.z += obj.z;
		return result;
	}
	float3 operator-(float3 const& obj)
	{
		float3 result(x, y, z);
		result.x -= obj.x;
		result.y -= obj.y;
		result.z -= obj.z;
		return result;
	}
	float3 operator*(float const& x1)
	{
		float3 result(x, y, z);
		result.x *= x1;
		result.y *= x1;
		result.z *= x1;
		return result;
	}
	float3 operator/(float const& x1)
	{
		float3 result(x, y, z);
		result.x /= x1;
		result.y /= x1;
		result.z /= x1;
		return result;
	}

	std::string ToString()
	{
		std::string output;
		output += std::to_string(x);
		output += ", ";
		output += std::to_string(y);
		output += ", ";
		output += std::to_string(z);
		return output;
	}

	float x = 0;
	float y = 0;
	float z = 0;
};


class Color
{
public:
	Color() { r = 0; b = 0; g = 0; a = 255; }
	Color(int r1, int g1, int b1, int a1) { r = r1; g = g1; b = b1; a = a1; }
	int32_t ToInt() { return a << 24 | r << 16 | g << 8 | b; }
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;
};
