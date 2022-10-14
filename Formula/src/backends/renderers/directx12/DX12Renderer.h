#pragma once

#include "renderer/Renderer.h"

// Import
ID3D12Device* g_pd3dDevice;

class DirectX12Renderer : public Renderer
{
public:	
	virtual void AllocateGPUResource(const void* initData = nullptr) override;
	virtual void CompileShaderProgram(const std::wstring path) override;
};

// Export
DirectX12Renderer DX12Renderer;