#pragma once
#include <string>

#include "Color.h"

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
	void operator+=(float2 const& obj)
	{
		x += obj.x;
		y += obj.y;
	}
	void operator-=(float2 const& obj)
	{
		x -= obj.x;
		y -= obj.y;
	}
	void operator*=(float const& x1)
	{
		x *= x1;
		y *= x1;
	}
	void operator/=(float const& x1)
	{
		x /= x1;
		y /= x1;
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
	void operator+=(float3 const& obj) 
	{
		x += obj.x;
		y += obj.y;
		z += obj.z;
	}
	void operator-=(float3 const& obj)
	{
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
	}
	void operator*=(float const& x1)
	{
		x *= x1;
		y *= x1;
		z *= x1;
	}
	void operator/=(float const& x1)
	{
		x /= x1;
		y /= x1;
		z /= x1;
	}

	friend float3 operator/(float f, float3 f3)
	{
		return float3(f / f3.x, f / f3.y, f / f3.z);
	}

	float2 xy() { return float2(x, y); }
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

	float Dot(float3 b) { return (x * b.x) + (y * b.y) + (z * b.z); }
	float3 Normalised() { 
		float length = sqrt((x * x) + (y * y) + (z * z));
		if (length == 0) { return float3(0, 0, 0); }
		return float3(x / length, y / length, z / length);
	}

	float x = 0;
	float y = 0;
	float z = 0;
};

class float4
{
public:
	float4() {}
	float4(float x1, float y1, float z1, float w1) { x = x1; y = y1; z = z1; w = w1; }
	float4(Color col) { x = col.r / 255.0f; y = col.g / 255.0f; z = col.b / 255.0f; w = col.a / 255.0f; }

	float4 operator+(float4 const& obj)
	{
		float4 result(x, y, z, w);
		result.x += obj.x;
		result.y += obj.y;
		result.z += obj.z;
		result.w += obj.w;
		return result;
	}
	float4 operator-(float4 const& obj)
	{
		float4 result(x, y, z, w);
		result.x -= obj.x;
		result.y -= obj.y;
		result.z -= obj.z;
		result.w -= obj.w;
		return result;
	}
	float4 operator*(float const& x1)
	{
		float4 result(x, y, z, w);
		result.x *= x1;
		result.y *= x1;
		result.z *= x1;
		return result;
	}
	float4 operator/(float const& x1)
	{
		float4 result(x, y, z, w);
		result.x /= x1;
		result.y /= x1;
		result.z /= x1;
		result.w /= x1;
		return result;
	}
	void operator+=(float4 const& obj)
	{
		x += obj.x;
		y += obj.y;
		z += obj.z;
		w += obj.w;
	}
	void operator-=(float4 const& obj)
	{
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		w -= obj.w;
	}
	void operator*=(float const& x1)
	{
		x *= x1;
		y *= x1;
		z *= x1;
		w *= x1;
	}
	void operator/=(float const& x1)
	{
		x /= x1;
		y /= x1;
		z /= x1;
		w /= x1;
	}
	friend float4 operator/(float f, float4 f3)
	{
		return float4(f / f3.x, f / f3.y, f / f3.z, f / f3.w);
	}
	float4 operator*(Color const& col) { return float4(x * col.r, y * col.g, z * col.b, w * col.a); }
	float4 operator*(float4 const& x1)
	{
		float4 result(x, y, z, w);
		result.x *= x1.x;
		result.y *= x1.y;
		result.z *= x1.z;
		result.w *= x1.w;
		return result;
	}

	float2 xy() { return float2(x, y); }
	std::string ToString()
	{
		std::string output;
		output += std::to_string(x);
		output += ", ";
		output += std::to_string(y);
		output += ", ";
		output += std::to_string(z);
		output += ", ";
		output += std::to_string(w);
		return output;
	}
	Color ToColor() { return Color(x*255, y*255, z*255, w*255); }

	float Dot(float4 b) { return (x * b.x) + (y * b.y) + (z * b.z) + (w * b.w); }
	float4 Normalised() {
		float length = sqrt((x * x) + (y * y) + (z * z) + (w * w));
		if (length == 0) { return float4(0, 0, 0, 0); }
		return float4(x / length, y / length, z / length, w / length);
	}

	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};




