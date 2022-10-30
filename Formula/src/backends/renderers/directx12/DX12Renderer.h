#pragma once

#include "renderer/Renderer.h"
#include "core/DX12App_UniformBuffer.h"	
#include "core/DX12App_Synchronizer.h"
#include "core/DX12App_SceneBuffer.h"
#include "core/DX12App_RootSigner.h"

// Impoprt from ImGui
extern int const NUM_FRAMES_IN_FLIGHT;
extern int const NUM_BACK_BUFFERS;

class DX12Renderer : public Renderer
{
public:	
	DX12Renderer();
	virtual ~DX12Renderer();

	void Init(ID3D12Device* device);

	// Graphics API Overloads
	virtual void RequestService(GraphicsService::Begin what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::PreProcess what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::UploadStaticResource what, const std::wstring& path, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::AllocateDynamicResource what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::UploadDynamicResource what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::SetRenderer what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::SetViewPort what, const int width, const int height, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::Enqueue what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::End what, const void* _opt_in_Info, void* _opt_out_Info) override;

private:
	ID3D12Device* m_Device;

	ComPtr<ID3D12DescriptorHeap> m_SrvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_SrvDescriptorsCPU[NUM_BACK_BUFFERS];
	D3D12_GPU_DESCRIPTOR_HANDLE m_SrvDescriptorsGPU[NUM_BACK_BUFFERS];
	UINT m_SrvDescriptorSize;

	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_RtvDescriptors[NUM_BACK_BUFFERS];
	UINT m_RtvDescriptorSize;

	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_DsvDescriptor;
	UINT m_DsvDescriptorSize;

	WaitSync m_WaitSync;

	//----------------- Renderer's Database Heap -----------------//	
	//------------------------------------------------------------//	
	
	// Mesh buffers
	HashTable<std::string,  Scope<MeshGeometry>> m_Meshes;
	
	// Scene frame contexts
	SceneFrameContext m_SceneFrameContexts[NUM_BACK_BUFFERS];
	SceneFrameContext* m_CurrSceneFrameContext;
	int m_CurrSceneFrameIndex = 0;

	// Scene buffers
	RenderTexture m_SceneBuffers[NUM_BACK_BUFFERS];
	
	// Uniform buffers containing 1) Engine-level built-in property data (e.g. transform, camera, light, etc)
	//							  2) Shader-level built-in property data (e.g. color, emission, etc)
	UniformManager m_UniformBuffers;

	// Underlying shader resouce binding scheme
	RootSignature m_RootSignature;

	// Image textures 
	HashTable<std::string, Scope<ImageTexture>> m_ImageTex;

	// Mesh render pipelines per shader	
	HashTable<std::string, ComPtr<ID3D12PipelineState>> m_MeshRendererPSOs;

	// Line render pipelines per shader	
	HashTable<std::string, ComPtr<ID3D12PipelineState>> m_LineRendererPSOs;
	
	// Particle render pipelines per shader
	HashTable<std::string, ComPtr<ID3D12PipelineState>> m_ParticleRendererPSOs;
};
