#include "Camera.h"
#include <iostream>
void Camera::Update()
{

}

float3 Camera::ToLocalPoint(float3 worldPoint)
{
	return (TransformVector(inverseBasisVectors, worldPoint - position));
}

float3 Camera::VertexToScreen(float3 vertex, Transform* transform, float2 numPixels)
{
	float3 worldPoint = transform->ToWorldPoint(vertex);
	//std::cout << vertex.ToString() << " : " << worldPoint.ToString() << std::endl;
	float3 viewPoint = ToLocalPoint(worldPoint);

	float screenHeight_world = tan(Fov / 2) * 2;
	float pixelsPerWorldUnit = numPixels.y / screenHeight_world / viewPoint.z;

	float2 pixelOffset = float2(viewPoint.x, viewPoint.y) * pixelsPerWorldUnit;
	float2 screenPoint = numPixels / 2 + pixelOffset;

	return float3(screenPoint.x, screenPoint.y, viewPoint.z);
}
