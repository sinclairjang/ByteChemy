#pragma once

#include "renderer/Renderer.h"
#include "backends/renderers/directx12/core/d3dx12_rootsigner.h"
#include "backends/renderers/directx12/core/d3dx12_rscalloc.h"
#include "backends/renderers/directx12/core/d3dx12_scenebuf.h"

// Data mirroring ImGui main framework
static int const NUM_FRAMES_IN_FLIGHT = 3;
static int const NUM_BACK_BUFFERS = 3;

class DX12Renderer : public Renderer
{
	// Aliases Suite



public:	
	// Graphics API Overloads
	virtual void RequestService(GraphicsService::AllocateGPUMemory allocWhat, const std::wstring& path, const void* initData, void* outInfo) override;
	virtual void RequestService(GraphicsService::CreateGPUProgram bindWhat, const std::wstring& path, void* outInfo) override;
	virtual void RequestService(GraphicsService::SetRenderTarget renderWhere, const size_t width, const size_t height, void* outInfo) override;
	virtual void RequestService(GraphicsService::Render drawHow, void* outInfo) override;

private:
	// Mesh buffer
	std::unordered_map<std::string,  std::unique_ptr<MeshGeometry>> m_MeshObjects;
	
	// Scene buffer
	std::unique_ptr<RenderTexture> m_SceneBuffers[NUM_BACK_BUFFERS];
	
	ComPtr<ID3D12DescriptorHeap> m_SrvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_SrvDescriptors[NUM_BACK_BUFFERS];
	
	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_RtvDescriptors[NUM_BACK_BUFFERS];

	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_DsvDescriptor;

	// Texture uploader
	std::unordered_map<std::string, std::unique_ptr<ImageTexture>> m_TexUploader;

	// Engine-level built-in property data (e.g. transform, camera, light, etc)
	std::unordered_map<std::string, ComPtr<ID3D12Resource>> m_EngineProperties;

	// Shader-level built-in property data (e.g. color, emission, etc)
	std::unordered_map<std::string, ComPtr<ID3D12Resource>> m_ShadingProperties;

	// Mesh render pipelines per shader	
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_MeshRendererPSOs;

	// Line render pipelines per shader	
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_LineRendererPSOs;
	
	// Particle render pipelines per shader
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_ParticleRendererPSOs;
};
