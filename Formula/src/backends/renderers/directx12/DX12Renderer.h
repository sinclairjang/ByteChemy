#pragma once

#include "renderer/Renderer.h"

// Import
ID3D12Device* g_pd3dDevice;

struct MeshBufferView
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	D3D12_INDEX_BUFFER_VIEW ibv;
};

class DirectX12Renderer : public Renderer
{
public:	
	// Graphics API Overloads
	virtual void RequestService(GraphicsService::AllocateGPUMemory allocWhat, _In_ const void* initData, _Out_ SafelyCopyablePointer<void> outInfo) override;
	virtual void RequestService(GraphicsService::BindShaderProgram bindWhere, _In_ const void* shaderInfo, _Out_ void* outInfo) override;
	virtual void RequestService(GraphicsService::Draw drawHow, const void* renderInfo) override;
};

// Export
extern DirectX12Renderer DX12Renderer;