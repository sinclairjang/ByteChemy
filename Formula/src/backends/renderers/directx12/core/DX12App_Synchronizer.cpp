#include "fm_pch.h"
#include "DX12App_Synchronizer.h"
#include "DX12App_ErrorHandler.h"

WaitSync::~WaitSync()
{
	// Note: The rest of resourcees is automatically managed by ComPtr.
	CloseHandle(m_Event);
}

void WaitSync::Init(ID3D12Device* device)
{
	m_Device = device;

	ThrowIfFailed(m_Device->CreateFence(
		0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.GetAddressOf())
	));

	m_Event = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);  // Auto-reset event object
	FM_ASSERT(m_Event != NULL);

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	ThrowIfFailed(m_Device->CreateCommandQueue(
		&queueDesc, IID_PPV_ARGS(m_CmdQueue.GetAddressOf())
	));

	ThrowIfFailed(m_Device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_CmdAlloc.GetAddressOf())
	));

	ThrowIfFailed(m_Device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAlloc.Get(), NULL, IID_PPV_ARGS(m_CmdList.GetAddressOf())
	));
}

ComPtr<ID3D12GraphicsCommandList>& WaitSync::Begin()
{
	// Delegate the graphics work submissions to the client
	return m_CmdList;
}

void WaitSync::Mark()
{
	++m_CurrentFenceValue;

	ThrowIfFailed(m_CmdQueue->Signal(m_Fence.Get(), m_CurrentFenceValue));
}

void WaitSync::Wait()
{
	ThrowIfFailed(m_Fence->SetEventOnCompletion(m_CurrentFenceValue, m_Event));
	
	WaitForSingleObject(m_Event, INFINITE);
}

void WaitSync::Flush()
{
	ThrowIfFailed(m_CmdList->Close());

	m_CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)m_CmdList.GetAddressOf());
	ThrowIfFailed(m_CmdQueue->Signal(m_Fence.Get(), 1));

	m_Fence->SetEventOnCompletion(1, m_Event);
	WaitForSingleObject(m_Event, INFINITE);
}

