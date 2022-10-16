#pragma once

#include "renderer/Renderer.h"

// Import
extern ID3D12Device* g_pd3dDevice;

struct MeshBufferView
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	D3D12_INDEX_BUFFER_VIEW ibv;
};

class DirectX12Renderer : public Renderer
{
	using RootCodeName = std::string;

public:	
	// Graphics API Overloads
	virtual void RequestService(GraphicsService::AllocateGPUMemory allocWhat, _In_ SafelyCopyablePointer<const void> initData, _Out_ SafelyCopyablePointer<void> outInfo) override;
	virtual void RequestService(GraphicsService::BindShaderProgram bindHow, const std::wstring& path, _Out_ SafelyCopyablePointer<void> outInfo) override;
	virtual void RequestService(GraphicsService::Draw drawHow, const void* renderInfo) override;

private:
	HashTable<RootCodeName, RootSignature> m_RootSigTable;
};

// Export
extern DirectX12Renderer DX12Renderer;