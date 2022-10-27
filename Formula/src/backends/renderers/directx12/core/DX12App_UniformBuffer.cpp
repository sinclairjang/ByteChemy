#include "fm_pch.h"
#include "DX12App_UniformBuffer.h"

#include "DX12App_ErrorHandler.h"
#include "utils/Log.h"

void UniformManager::Init(ID3D12Device* device)
{
	m_Device = device;

	for (int frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
	{
		m_UniformFrameResources[frameIndex].Init(m_Device);
	}

	// Cache
	m_MainPropsAlignment = m_UniformFrameResources[0].MainProps.GetElementByteSize();
	m_MainPassAlignment = m_UniformFrameResources[0].MainPass.GetElementByteSize();

	m_UnlitPropsAlignment = m_UniformFrameResources[0].UnlitProps.GetElementByteSize();
	m_UnlitPassAlignment = m_UniformFrameResources[0].UnlitPass.GetElementByteSize();

	m_CbvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	{
		D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = NUM_FRAMES_IN_FLIGHT;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_Device->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(m_MainPassHeap.GetAddressOf())
		));
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = NUM_FRAMES_IN_FLIGHT;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_Device->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(m_UnlitPassHeap.GetAddressOf())
		));
	}

	for (int frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
	{
		{
			m_UniformFrameResources[frameIndex].MainPass.SetNumElements(1);  // Recall: One cbuffer per frame
			m_UniformFrameResources[frameIndex].MainPass.Map();


			D3D12_GPU_VIRTUAL_ADDRESS mainPassAddress = m_UniformFrameResources[frameIndex].MainPass.Resource()->GetGPUVirtualAddress();

			auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_MainPassHeap->GetCPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(frameIndex, m_CbvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
			cbvDesc.BufferLocation = mainPassAddress;
			cbvDesc.SizeInBytes = m_UniformFrameResources[frameIndex].MainPass.GetElementByteSize();

			m_Device->CreateConstantBufferView(&cbvDesc, cbvHandle);
		}

		{
			m_UniformFrameResources[frameIndex].UnlitPass.SetNumElements(1);  // Recall: One cbuffer per frame
			m_UniformFrameResources[frameIndex].UnlitPass.Map();


			D3D12_GPU_VIRTUAL_ADDRESS unlitPassAddress = m_UniformFrameResources[frameIndex].UnlitPass.Resource()->GetGPUVirtualAddress();

			int heapIndex = frameIndex;

			auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_UnlitPassHeap->GetCPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(heapIndex, m_CbvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
			cbvDesc.BufferLocation = unlitPassAddress;
			cbvDesc.SizeInBytes = m_UniformFrameResources[frameIndex].UnlitPass.GetElementByteSize();

			m_Device->CreateConstantBufferView(&cbvDesc, cbvHandle);
		}
	}
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


void UniformManager::ResizeMainPropBuffers(UINT numMainProps)
{
	
	D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
	cbvHeapDesc.NumDescriptors = m_NumMainProps;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(m_Device->CreateDescriptorHeap(
		&cbvHeapDesc, IID_PPV_ARGS(m_MainPropsHeap.GetAddressOf())
	));
	
	for (UINT frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
	{
		auto& mainPropsBuffer = m_UniformFrameResources[frameIndex].MainProps;
		mainPropsBuffer.SafeRelease();
		mainPropsBuffer.SetNumElements(numMainProps);
		mainPropsBuffer.Map();

		for (UINT i = 0; i < m_NumMainProps; ++i)
		{
			D3D12_GPU_VIRTUAL_ADDRESS mainPropsAddress = mainPropsBuffer.Resource()->GetGPUVirtualAddress();
			mainPropsAddress += (UINT64)i * m_MainPropsAlignment;

			UINT heapIndex = frameIndex * m_NumMainProps + i;
			auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_MainPropsHeap->GetCPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(heapIndex, m_CbvDescriptorSize);
			
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
			cbvDesc.BufferLocation = mainPropsAddress;
			cbvDesc.SizeInBytes = m_MainPropsAlignment;

			m_Device->CreateConstantBufferView(&cbvDesc, cbvHandle);
		}
	}
}



void UniformManager::ResizeShadingPropBuffers(ShadingType type, UINT numUnlitProps)
{
	if (type == ShadingType::UNLIT)
	{
		D3D12_DESCRIPTOR_HEAP_DESC	cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = numUnlitProps;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_Device->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(m_UnlitPropsHeap.GetAddressOf())
		));

		for (UINT frameIndex = 0; frameIndex < NUM_FRAMES_IN_FLIGHT; ++frameIndex)
		{
			auto& unlitPropsBuffer = m_UniformFrameResources[frameIndex].UnlitProps;
			unlitPropsBuffer.SafeRelease();
			unlitPropsBuffer.SetNumElements(numUnlitProps);
			unlitPropsBuffer.Map();

			for (UINT i = 0; i < m_NumUnlitProps; ++i)
			{
				D3D12_GPU_VIRTUAL_ADDRESS mainPropsAddress = unlitPropsBuffer.Resource()->GetGPUVirtualAddress();
				mainPropsAddress += (UINT64)i * m_UnlitPropsAlignment;

				UINT heapIndex = frameIndex * m_NumUnlitProps + i;
				auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
					m_UnlitPropsHeap->GetCPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(heapIndex, m_CbvDescriptorSize);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
				cbvDesc.BufferLocation = mainPropsAddress;
				cbvDesc.SizeInBytes = m_UnlitPropsAlignment;

				m_Device->CreateConstantBufferView(&cbvDesc, cbvHandle);
			}
		}
	}

	else
	{
		LOG_INFO("Only ShadingType(UNLIT) is supported for now");
	}
}

void UniformFrameResource::Init(ID3D12Device* device)
{
	MainProps.Init(device, true);
	MainPass.Init(device, true);
	UnlitPass.Init(device, true);
	UnlitProps.Init(device, true);
}
