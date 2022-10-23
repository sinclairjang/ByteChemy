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

//struct UnlitShadingPass			// -> register(b2)
//{
//	//TEMP
//};

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

// UniformManager is responsible for 1) allocations and resizing of upload buffers upon creations of Entities
//									 2) managing buffer indices to map Entites to upload buffers
//									 3) flipping upload buffers upon new frame

struct FrameResource
{
	FrameResource(ID3D12CommandAllocator* cmdAlloc);

	//TEMP
	UploadBufAllocator<EngineObjectProperty> m_EngineObjProps;
	UploadBufAllocator<EnginPass> m_EnginePass;
	UploadBufAllocator<UnlitShadingProperty> m_UnlitProps;

private:
	ID3D12CommandAllocator* m_CmdAlloc;
};

class UniformManager
{

private:
	static const int NumFrameResources = NUM_FRAMES_IN_FLIGHT;

	std::vector<std::unique_ptr<FrameResource>> m_FrameResources;
	
	FrameResource* m_CurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
};