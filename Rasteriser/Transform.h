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
	float Pitch;

	float3 position;

	float3 ToWorldPoint(float3 p)
	{
		BasisVectors basisVectors = GetBasisVectors();
		return (TransformVector(basisVectors, p) + position);
	}
	
	BasisVectors GetBasisVectors()
	{
		BasisVectors out;
		BasisVectors yaw;
		BasisVectors pitch;

		yaw.ihat = float3(cosf(Yaw), 0, sinf(Yaw));
		yaw.jhat = float3(0, 1, 0);
		yaw.khat = float3(-sinf(Yaw), 0, cos(Yaw));

		pitch.ihat = float3(1, 0, 0);
		pitch.jhat = float3(0 ,cosf(Pitch), -sinf(Pitch));
		pitch.khat = float3(0 ,sinf(Pitch), cos(Pitch));

		out.ihat = TransformVector(yaw, pitch.ihat);
		out.jhat = TransformVector(yaw, pitch.jhat);
		out.khat = TransformVector(yaw, pitch.khat);

		
		return out;
	}

	float3 TransformVector(BasisVectors bVecs, float3 p)
	{
		return bVecs.ihat * p.x + bVecs.jhat * p.y + bVecs.khat * p.z;
	}
};

