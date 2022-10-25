#pragma once

#include "renderer/Renderer.h"
#include "core/DX12App_UniformBuffer.h"	

// Data mirroring ImGui main framework
static int const NUM_FRAMES_IN_FLIGHT = 3;
static int const NUM_BACK_BUFFERS = 3;

// Forward decalarations
class RenderTexture;
class RootSignature;

// Backend render data associated with a game object represented by entt::entity
struct RenderItem
{
	RenderItem() = default;

	std::unique_ptr<EngineObjectProperty> EngineObjProp = nullptr;
	
	union
	{
		std::unique_ptr<UnlitShadingProperty> UnlitProp = nullptr;
		//std::unique_ptr<ParticleShadingProperty> ParticleProp = nullptr;
		//std::unique_ptr<StandardShadingProperty> StandardProp = nullptr;
	};

	int NumDirtyFrames = NUM_FRAMES_IN_FLIGHT;
	
	UINT EngineUniformIdx = -1;
	UINT ShadingUniformIdx = -1;

	//Note: This pointer is stable (i.e. valid as long as DX12Renderer object lifetime).
	MeshGeometry* meshGeo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY primType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

class DX12Renderer : public Renderer
{
public:	
	// Graphics API Overloads
	virtual void RequestService(GraphicsService::PreProcess what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::LoadResource what, const std::wstring& path, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::AllocateResource what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::Update what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::SetRenderer what, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::SetViewPort what, const int width, const int height, const void* _opt_in_Info, void* _opt_out_Info) override;
	virtual void RequestService(GraphicsService::Enqueue what, const void* _opt_in_Info, void* _opt_out_Info) override;

private:
	ComPtr<ID3D12DescriptorHeap> m_SrvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_SrvDescriptors[NUM_BACK_BUFFERS];

	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_RtvDescriptors[NUM_BACK_BUFFERS];

	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_DsvDescriptor;

	//----------------- Renderer's Database -----------------//
	//-------------------------------------------------------//
	
	// Mesh buffers
	HashTable<std::string,  std::unique_ptr<MeshGeometry>> m_MeshObjects;
	
	// Render objects
	HashTable<entt::entity, RenderItem> m_RenderObjects;

	// Scene buffers
	Scope<RenderTexture> m_SceneBuffers[NUM_BACK_BUFFERS];
	
	// Uniform buffers containing 1) Engine-level built-in property data (e.g. transform, camera, light, etc)
	//							  2) Shader-level built-in property data (e.g. color, emission, etc)
	Scope<UniformManager> m_UniformBuffers;

	// Underlying shader resouce binding scheme
	Scope<RootSignature> m_RootSignature;

	// Image textures 
	HashTable<std::string, std::unique_ptr<ImageTexture>> m_ImageTex;

	// Mesh render pipelines per shader	
	HashTable<std::string, ComPtr<ID3D12PipelineState>> m_MeshRendererPSOs;

	// Line render pipelines per shader	
	HashTable<std::string, ComPtr<ID3D12PipelineState>> m_LineRendererPSOs;
	
	// Particle render pipelines per shader
	HashTable<std::string, ComPtr<ID3D12PipelineState>> m_ParticleRendererPSOs;
	

};

