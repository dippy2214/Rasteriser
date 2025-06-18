#pragma once
class BaseShader
{
public:
	virtual char* RunShader(void* shaderParameters, int dataSize) = 0;
};

