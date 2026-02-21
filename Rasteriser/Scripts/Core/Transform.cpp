#include "Transform.h"

BasisVectors Transform::GetBasisVectors()
{
	BasisVectors out;

	float cosYaw = cosf(Yaw);
	float sinYaw = sinf(Yaw);
	float cosPitch = cosf(Pitch);
	float sinPitch = sinf(Pitch);

	out.ihat = float3(cosYaw, 0, sinYaw);
	out.jhat = float3(sinPitch * sinYaw, cosPitch, -cosYaw * sinPitch);
	out.khat = float3(-sinYaw * cosPitch, sinPitch, cosYaw * cosPitch);

	return out;
}

BasisVectors Transform::GetInverseBasisVectors()
{
	BasisVectors out;

	float cosYaw = cosf(Yaw);
	float sinYaw = sinf(Yaw);
	float cosPitch = cosf(Pitch);
	float sinPitch = sinf(Pitch);

	out.ihat = float3(cosYaw, sinPitch * sinYaw, -sinYaw * cosPitch);
	out.jhat = float3(0, cosPitch, sinPitch);
	out.khat = float3(sinYaw, -cosYaw * sinPitch, cosYaw * cosPitch);

	return out;
}

Transform::Transform()
{
	basisVectors = GetBasisVectors();
	inverseBasisVectors = GetInverseBasisVectors();
}

void Transform::SetRotationX(float rotation)
{
	Pitch = rotation;

	basisVectors = GetBasisVectors();
	inverseBasisVectors = GetInverseBasisVectors();
}

void Transform::SetRotationY(float rotation)
{
	Yaw = rotation;

	basisVectors = GetBasisVectors();
	inverseBasisVectors = GetInverseBasisVectors();
}
