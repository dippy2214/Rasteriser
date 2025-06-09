#include "Camera.h"

void Camera::Update()
{

}

float3 Camera::ToLocalPoint(float3 worldPoint)
{
	BasisVectors basisVectors = GetInverseBasisVectors();
	return (TransformVector(basisVectors, worldPoint - position));
	//return worldPoint - position;
}
