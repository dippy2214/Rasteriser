#pragma once
#include "FloatTypes.h"

class Transform
{

	struct BasisVectors
	{
		float3 ihat;
		float3 jhat;
		float3 khat;
	};
public:
	float Yaw;

	float3 ToWorldPoint(float3 p)
	{
		BasisVectors basisVectors = GetBasisVectors();
		return TransformVector(basisVectors, p);
	}
	
	BasisVectors GetBasisVectors()
	{
		BasisVectors out;
		out.ihat = float3(cosf(Yaw), 0, sinf(Yaw));
		out.jhat = float3(0, 1, 0);
		out.khat = float3(-sinf(Yaw), 0, cos(Yaw));
		return out;
	}

	float3 TransformVector(BasisVectors bVecs, float3 p)
	{
		return bVecs.ihat * p.x + bVecs.jhat * p.y + bVecs.khat * p.z;
	}
};

