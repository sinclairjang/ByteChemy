#pragma once

//  Uniform Memory Management 
// 
//	Group uniforms based on	1) update frequencies (e.g. per objects or render pass etc)
//							2) render pipelines (e.g. unlit or standard etc)
//							3) commonality with respect to shaders (e.g. engine or otherwise)

#include "DX12App_ResourceAllocator.h"
#include "backends/renderers/directx12/template/DX12App_BindEntry.h"

using namespace DirectX;

// Import from ImGui main framework
extern ID3D12Device* g_pd3dDevice;

// Data mirroring ImGui main framework
static int const NUM_FRAMES_IN_FLIGHT = 3;
static int const NUM_BACK_BUFFERS = 3;

// UniformManager is responsible for 1) allocations and resizing of upload buffers upon creations of Entities
//									 2) managing buffer indices to map Entites to upload buffers
//									 3) flipping upload buffers upon new frame

struct UniformFrameResource
{
    UniformFrameResource() = default;

	//TEMP
    void Init(ID3D12Device* device);

	UploadBufAllocator<EngineObjectProperty> MainProps;
	UploadBufAllocator<EnginPass> MainPass;
    UploadBufAllocator<UnlitShadingPass> UnlitPass;
	UploadBufAllocator<UnlitShadingProperty> UnlitProps;
};

class UniformManager
{
    // Uniform buffer index manager
    //- On Entity being created, it is assigned an index to its object buffer via GetMainPropBufferIdx
    //- On Shader being attached to Entity, It is assigned an index to its property buffer via GetShadingPropBufferIdx
    
    //- On Entity being destroyed, it pushes the assigned index back to the underlying index queue
    //- On Shader being detached from Entity, it pushes the assigned index back to the underlying index queue

public:
    // Value-initialize uniform frame resources
    UniformManager() = default;
    
    void Init(ID3D12Device* device);

    void AllocateMainPassBuffers();
    void AllocateUnlitPassBuffers();

    // Create and grow uniform buffers if index stack is depleted
    UINT64 GetMainPropBufferIdx();
    UINT64 GetShadingPropBufferIdx(ShadingType type);

private:
    void ResizeMainPropBuffers(UINT numMainProps);
    void ResizeShadingPropBuffers(ShadingType type, UINT numUnlitProps);
        
private:    
	UniformFrameResource m_UniformFrameResources[NUM_FRAMES_IN_FLIGHT];

    UINT m_CbvDescriptorSize = 0;

private:
    ID3D12Device* m_Device;

    int m_MainPropsAlignment = 0;
    int m_MainPassAlignment = 0;
    // Standard, Particle ...

    int m_UnlitPropsAlignment = 0;
    int m_UnlitPassAlignment = 0;
    // Standard, Particle ...
    
    std::queue<UINT64> m_MainPropIdxQueue;
    std::queue<UINT64> m_UnlitPropIdxQueue;
    // Standard, Particle ...
    
    UINT m_NumMainProps = 0;
    UINT m_NumUnlitProps = 0;
    // Standard, Particle ...

    ComPtr<ID3D12DescriptorHeap> m_MainPropsHeap;
    ComPtr<ID3D12DescriptorHeap> m_MainPassHeap;
    // Standard, Particle ...
    
    ComPtr<ID3D12DescriptorHeap> m_UnlitPropsHeap;
    ComPtr<ID3D12DescriptorHeap> m_UnlitPassHeap;
    // Standard, Particle ...
};