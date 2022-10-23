#include "fm_pch.h"
#include "DX12App_UniformBuffer.h"

FrameResource::FrameResource(ID3D12CommandAllocator* cmdAlloc) 
	: m_CmdAlloc(cmdAlloc)
{
}
