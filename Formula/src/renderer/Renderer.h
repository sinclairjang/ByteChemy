#pragma once
class Renderer
{
public:
	virtual ~Renderer() = 0;
	virtual void AllocateGPUResource() = 0;
	virtual void CompileShaderProgram(const std::wstring path) = 0;
};


