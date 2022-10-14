#pragma once
class Renderer
{
public:
	virtual ~Renderer() {}
	virtual void AllocateGPUResource(const void* initData) = 0;
	virtual void CompileShaderProgram(const std::wstring path) = 0;
};


