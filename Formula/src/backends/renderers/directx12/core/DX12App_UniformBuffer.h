#pragma once

#include "DX12App_ResourceAllocator.h"

using namespace DirectX;

// Data mirroring ImGui main framework
static int const NUM_FRAMES_IN_FLIGHT = 3;
static int const NUM_BACK_BUFFERS = 3;

//	Group uniforms based on	1) update frequencies (e.g. per objects or render pass etc.)
//							2) render pipelines (e.g. unlit or standard etc.)
//							3) commonality with respect to shaders (e.g. engine or otherwise)

struct EngineObjectProperty		// -> register(b0)
{
	//TEMP
	XMFLOAT4X4 gWorld = FMMat4(1);	// == 4x4 identity matrix
};

struct EnginPass					// -> register(b1)
{
	//TEMP
	XMFLOAT4X4 gView;
	XMFLOAT4X4 gInvView;
	XMFLOAT4X4 gProj;
	XMFLOAT4X4 gInvProj;
	XMFLOAT4X4 gViewProj;
	XMFLOAT4X4 gInvViewProj;
	XMFLOAT3 gEyePosW;
	float _Pad1;
};

enum class ShadingType : UINT8
{
    UNLIT,
    STANDARD,
};

//DEBUG MARKER: should I supply placeholder data?
struct UnlitShadingPass			// -> register(b2)
{
	//TEMP
    XMFLOAT4 _Placeholder;
};

struct UnlitShadingProperty		// -> register(b3)
{
	//TEMP
	XMFLOAT4 gColor;
};

//TODO:
//struct StandardShadingPass		// -> register(b2)
//{
//
//};
//
//struct StandardShadingProperties	// -> register(b3)
//{
//
//};


/*
 // Create and grow vertex/index buffers if needed
    if (fr->VertexBuffer == NULL || fr->VertexBufferSize < draw_data->TotalVtxCount)
    {
        SafeRelease(fr->VertexBuffer);
        fr->VertexBufferSize = draw_data->TotalVtxCount + 5000;
        D3D12_HEAP_PROPERTIES props;
        memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
        props.Type = D3D12_HEAP_TYPE_UPLOAD;
        props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC desc;
        memset(&desc, 0, sizeof(D3D12_RESOURCE_DESC));
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = fr->VertexBufferSize * sizeof(ImDrawVert);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;
        if (bd->pd3dDevice->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&fr->VertexBuffer)) < 0)
            return;
    }
*/

// UniformManager is responsible for 1) allocations and resizing of upload buffers upon creations of Entities
//									 2) managing buffer indices to map Entites to upload buffers
//									 3) flipping upload buffers upon new frame


struct FrameResource
{
	//TEMP
	UploadBufAllocator<EngineObjectProperty> m_MainProps;
	UploadBufAllocator<EnginPass> m_MainPass;
	UploadBufAllocator<UnlitShadingProperty> m_UnlitProps;
};

class UniformManager
{
    // Uniform buffer index manager
    //- On Entity being created, it is assigned an index to its object buffer via GetMainPropBufferIdx
    //- On Shader being attached to Entity, It is assigned an index to its property buffer via GetShadingPropBufferIdx

public:
    UniformManager() = default;

    // Create and grow uniform buffers if index stack is empty
    UINT64 GetMainPropBufferIdx();
    UINT64 GetShadingPropBufferIdx(ShadingType type);

private:
    void CreateMainPropBuffers();
    void CerateMainPassBuffers();
    
    void CreateShadingPropBuffers(ShadingType type);
    void CreateShadingPassBuffers(ShadingType type);

    void ResizeMainPropBuffers();
    void ResizeMainPassBuffers();
    
    void ResizeShadingPropBuffers(ShadingType type);
    void ResizeShadingPassBuffers(ShadingType type);

private:
    static const int NumFrameResources = NUM_FRAMES_IN_FLIGHT;
    
	std::vector<std::unique_ptr<FrameResource>> m_FrameResources;
	
    FrameResource* m_CurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
    
    std::stack<UINT64> m_MainPropIdxStack;
    std::stack<UINT64> m_UnlitPropIdxStack;
   
    //TODO:
    // std::stack<UINT> m_StandardPropIdxStack;

    ComPtr<ID3D12DescriptorHeap> m_MainPropHeap;
    ComPtr<ID3D12DescriptorHeap> m_MainPassHeap;

    ComPtr<ID3D12DescriptorHeap> m_UnlitPropHeap;
    ComPtr<ID3D12DescriptorHeap> m_UnlitPassHeap;

    //TODO:
    //ComPtr<ID3D12DescriptorHeap> m_StandardPropHeap;
    //ComPtr<ID3D12DescriptorHeap> m_StandardPassHeap;
};