#pragma once
class BaseShader
{
public:
	virtual void* RunShader(void* shaderParameters, int dataSize) = 0;
};

