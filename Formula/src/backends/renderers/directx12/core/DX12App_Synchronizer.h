#pragma once

class WaitSync
{
public:
	WaitSync() = default;
	~WaitSync();

	void Init(ID3D12Device* device);

	ComPtr<ID3D12GraphicsCommandList>& Begin();
	
	void Mark();
	void Wait();
	void Flush();

private:	
	ID3D12Device* m_Device;

	ComPtr<ID3D12CommandQueue> m_CmdQueue;
	ComPtr<ID3D12CommandAllocator> m_CmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> m_CmdList;
	ComPtr<ID3D12Fence> m_Fence;

	HANDLE	m_Event;  //DEBUG MARK

	UINT64 m_CurrentFenceValue = 0;
};