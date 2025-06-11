#pragma once
#include "FloatTypes.h"

struct BasisVectors
{
public:
	float3 ihat;
	float3 jhat;
	float3 khat;
};

class Transform
{
private:
	float Yaw = 0;
	float Pitch = 0;
	
	BasisVectors GetBasisVectors();
	BasisVectors GetInverseBasisVectors();
public:
	BasisVectors basisVectors;
	BasisVectors inverseBasisVectors;

	float3 position;
	Transform();

	float3 ToWorldPoint(float3 p)
	{
		return (TransformVector(basisVectors, p) + position);
	}
	
	void SetRotationX(float rotation);
	void SetRotationY(float rotation);
	void AddRotationX(float rotation) { SetRotationX(rotation + Pitch); }
	void AddRotationY(float rotation) { SetRotationY(rotation + Yaw); }
		

	float3 TransformVector(BasisVectors bVecs, float3 p)
	{
		return bVecs.ihat * p.x + bVecs.jhat * p.y + bVecs.khat * p.z;
	}
};

