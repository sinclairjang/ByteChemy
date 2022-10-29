#pragma once

#include "DX12App_ResourceAllocator.h"

using namespace DirectX;

// Import from ImGui main framework
extern ID3D12Device* g_pd3dDevice;

// Data mirroring ImGui main framework
static int const NUM_FRAMES_IN_FLIGHT = 3;
static int const NUM_BACK_BUFFERS = 3;

//	Group uniforms based on	1) update frequencies (e.g. per objects or render pass etc)
//							2) render pipelines (e.g. unlit or standard etc)
//							3) commonality with respect to shaders (e.g. engine or otherwise)

// This groups of uniforms will be bound to the common memory scheme (i.e. root signature) that defines the set of shader registers to provide consistent interface with the underlying hardware per draw call.

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
    STANDARD,
    UNLIT,
    PARTICLE,
};

//DEBUG MARKER: should I supply placeholder data?
struct UnlitShadingPass			// -> register(b2)
{
	//TEMP
    XMFLOAT4 _Placeholder;
};

struct ShadingProperty
{
    virtual ~ShadingProperty() = 0;
};

struct UnlitShadingProperty : public ShadingProperty		// -> register(b3)
{
    virtual ~UnlitShadingProperty();
	
    //TEMP
	XMFLOAT4 gColor;
};

//TODO:
//struct ParticleShadingPass		// -> register(b2)
//{
//
//};
//
//struct ParticleShadingProperties	// -> register(b3)
//{
//
//};
//struct StandardShadingPass		// -> register(b2)
//{
//
//};
//
//struct StandardShadingProperties	// -> register(b3)
//{
//
//};

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