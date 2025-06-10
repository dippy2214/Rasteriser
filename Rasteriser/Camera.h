#pragma once
#include "Transform.h"

class Camera : public Transform
{
public:
	float Fov = 30.0f;
	float camSpeed = 15.0f;

	void Update();
	float3 ToLocalPoint(float3 worldPoint);
	float3 VertexToScreen(float3 vertex, Transform* transform, float2 numPixels);
};

