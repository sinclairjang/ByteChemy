#pragma once

class WaitSync
{
public:
	WaitSync();
	~WaitSync();

	ID3D12GraphicsCommandList* Begin();
	void Flush();

private:
	ComPtr<ID3D12CommandQueue>			m_CmdQueue;
	ComPtr<ID3D12CommandAllocator>		m_CmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_CmdList;
	ComPtr<ID3D12Fence>					m_Fence;

	HANDLE	event;
};