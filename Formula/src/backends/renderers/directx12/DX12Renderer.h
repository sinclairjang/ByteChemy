#pragma once

#include "renderer/Renderer.h"
#include "backends/renderers/directx12/core/d3dx12_rootsigner.h"

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
	virtual void RequestService(GraphicsService::AllocateGPUMemory allocWhat, const void* initData, SafelyCopyablePointer<void> outInfo) override;
	virtual void RequestService(GraphicsService::BindShaderProgram usage, const std::wstring& path, SafelyCopyablePointer<void> outInfo) override;
	//virtual void RequestService(GraphicsService::Draw drawMode);

private:
	HashTable<RootCodeName, RootSignature> m_RootSigTable;
};

