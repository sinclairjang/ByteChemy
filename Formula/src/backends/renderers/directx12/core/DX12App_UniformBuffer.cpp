#include "fm_pch.h"
#include "DX12App_UniformBuffer.h"

#include "DX12App_ErrorHandler.h"
#include "utils/Log.h"

UniformManager::UniformManager()
{
	{
		D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = NUM_FRAMES_IN_FLIGHT;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(m_MainPassHeap.GetAddressOf())
		));
	}
	
	{
		D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = NUM_FRAMES_IN_FLIGHT;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(m_UnlitPassHeap.GetAddressOf())
		));
	}

	for (int frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
	{
		m_UniformFrameResources[frameIndex] = std::make_unique<UniformFrameResource>();
		
		{
			auto& mainPassBuffer = m_UniformFrameResources[frameIndex]->MainPass;
			mainPassBuffer.SetNumElements(1);  // Recall: One cbuffer per frame
			mainPassBuffer.Map();

			D3D12_GPU_VIRTUAL_ADDRESS mainPassAddress = mainPassBuffer.Resource()->GetGPUVirtualAddress();

			int heapIndex = frameIndex;
			SIZE_T cbvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_MainPassHeap->GetCPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(heapIndex, cbvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
			cbvDesc.BufferLocation = mainPassAddress;
			cbvDesc.SizeInBytes = mainPassBuffer.GetElementByteSize();

			g_pd3dDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);
		}

		{
			auto& unlitPassBuffer = m_UniformFrameResources[frameIndex]->UnlitPass;
			unlitPassBuffer.SetNumElements(1);  // Recall: One cbuffer per frame
			unlitPassBuffer.Map();

			D3D12_GPU_VIRTUAL_ADDRESS unlitPassAddress = unlitPassBuffer.Resource()->GetGPUVirtualAddress();

			int heapIndex = frameIndex;
			SIZE_T cbvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_UnlitPassHeap->GetCPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(heapIndex, cbvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
			cbvDesc.BufferLocation = unlitPassAddress;
			cbvDesc.SizeInBytes = unlitPassBuffer.GetElementByteSize();

			g_pd3dDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);
		}
	}
	
	// Cache
	m_MainPropsAlignment = m_UniformFrameResources[0]->MainProps.GetElementByteSize();
	m_MainPassAlignment = m_UniformFrameResources[0]->MainPass.GetElementByteSize();
	
	m_UnlitPropsAlignment = m_UniformFrameResources[0]->UnlitProps.GetElementByteSize();
	m_UnlitPassAlignment = m_UniformFrameResources[0]->UnlitPass.GetElementByteSize();

	m_CbvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

UINT64 UniformManager::GetMainPropBufferIdx()
{
	if (m_MainPropIdxQueue.empty())
	{
		std::deque<UINT64> deque(10);
		std::iota(deque.begin(), deque.end(), m_NumMainProps);

		m_MainPropIdxQueue = std::queue<UINT64>(std::move(deque));  // iter1: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], iter2: [10, 11, 12, 13, 14, 15, 16, 17, 18, 19], ...

		m_NumMainProps += 10;

		ResizeMainPropBuffers(m_NumMainProps);
	}
	
	UINT64 mainPropBufIdx = m_MainPropIdxQueue.front();
	m_MainPropIdxQueue.pop();

	return mainPropBufIdx;
	
}

UINT64 UniformManager::GetShadingPropBufferIdx(ShadingType type)
{
	if (type == ShadingType::UNLIT)
	{
		if (m_UnlitPropIdxQueue.empty())
		{
			std::deque<UINT64> deque(10);
			std::iota(deque.begin(), deque.end(), m_NumUnlitProps);

			m_UnlitPropIdxQueue = std::queue<UINT64>(std::move(deque));  // iter1: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], iter2: [10, 11, 12, 13, 14, 15, 16, 17, 18, 19], ...

			m_NumUnlitProps += 10;

			ResizeShadingPropBuffers(ShadingType::UNLIT, m_NumUnlitProps);
		}
	}

	else
	{
		LOG_INFO("Only ShadingType(UNLIT) is supported for now");
		return -1;  // Reserved invalid index
	}

	UINT64 unlitPropBufIdx = m_UnlitPropIdxQueue.front();
	m_UnlitPropIdxQueue.pop();

	return unlitPropBufIdx;
}


void UniformManager::ResizeMainPropBuffers(UINT64 numMainProps)
{
	
	D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
	cbvHeapDesc.NumDescriptors = m_NumMainProps;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
		&cbvHeapDesc, IID_PPV_ARGS(m_MainPropsHeap.GetAddressOf())
	));
	
	for (UINT64 frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
	{
		auto& mainPropsBuffer = m_UniformFrameResources[frameIndex]->MainProps;
		mainPropsBuffer.SafeRelease();
		mainPropsBuffer.SetNumElements(numMainProps);
		mainPropsBuffer.Map();

		for (UINT64 i = 0; i < m_NumMainProps; ++i)
		{
			D3D12_GPU_VIRTUAL_ADDRESS mainPropsAddress = mainPropsBuffer.Resource()->GetGPUVirtualAddress();
			mainPropsAddress += i * m_MainPropsAlignment;

			UINT64 heapIndex = frameIndex * m_NumMainProps + i;
			auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_MainPropsHeap->GetCPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(heapIndex, m_CbvDescriptorSize);
			
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
			cbvDesc.BufferLocation = mainPropsAddress;
			cbvDesc.SizeInBytes = m_MainPropsAlignment;

			g_pd3dDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);
		}
	}
}



void UniformManager::ResizeShadingPropBuffers(ShadingType type, UINT64 numUnlitProps)
{
	if (type == ShadingType::UNLIT)
	{
		D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = numUnlitProps;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(m_UnlitPropsHeap.GetAddressOf())
		));

		for (int frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
		{
			auto& unlitPropsBuffer = m_UniformFrameResources[frameIndex]->UnlitProps;
			unlitPropsBuffer.SafeRelease();
			unlitPropsBuffer.SetNumElements(numUnlitProps);
			unlitPropsBuffer.Map();

			for (UINT64 i = 0; i < m_NumUnlitProps; ++i)
			{
				D3D12_GPU_VIRTUAL_ADDRESS mainPropsAddress = unlitPropsBuffer.Resource()->GetGPUVirtualAddress();
				mainPropsAddress += i * m_UnlitPropsAlignment;

				UINT64 heapIndex = frameIndex * m_NumUnlitProps + i;
				auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
					m_UnlitPropsHeap->GetCPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(heapIndex, m_CbvDescriptorSize);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
				cbvDesc.BufferLocation = mainPropsAddress;
				cbvDesc.SizeInBytes = m_UnlitPropsAlignment;

				g_pd3dDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);
			}
		}
	}

	else
	{
		LOG_INFO("Only ShadingType(UNLIT) is supported for now");
	}
}

