#include "fm_pch.h"
#include "DX12App_UniformBuffer.h"

#include "utils/Log.h"

UniformManager::UniformManager()
{
	for (int i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
	{
		m_UniformFrameResources[i] = std::make_unique<UniformFrameResource>();
		
		m_UniformFrameResources[i]->MainPass.SetNumElements(NUM_FRAMES_IN_FLIGHT);
		m_UniformFrameResources[i]->MainPass.Map();

		m_UniformFrameResources[i]->UnlitPass.SetNumElements(NUM_FRAMES_IN_FLIGHT);
		m_UniformFrameResources[i]->UnlitPass.Map();
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
		LOG_INFO("Other shading types are not yet supported");
		return -1;  // Reserved invalid index
	}

	UINT64 unlitPropBufIdx = m_UnlitPropIdxQueue.front();
	m_UnlitPropIdxQueue.pop();

	return unlitPropBufIdx;
}


void UniformManager::ResizeMainPropBuffers(UINT64 numMainProps)
{
	for (int i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
	{
		m_UniformFrameResources[i]->MainProps.SafeRelease();
		
		m_UniformFrameResources[i]->MainProps.SetNumElements(numMainProps);
		
		m_UniformFrameResources[i]->MainProps.Map();
	}
}



void UniformManager::ResizeShadingPropBuffers(ShadingType type, UINT64 numUnlitProps)
{
	if (type == ShadingType::UNLIT)
	{
		for (int i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
		{
			m_UniformFrameResources[i]->UnlitProps.SafeRelease();

			m_UniformFrameResources[i]->UnlitProps.SetNumElements(numUnlitProps);

			m_UniformFrameResources[i]->UnlitProps.Map();
		}
	}

	else
	{
		LOG_INFO("Other shading types are not yet supported");
	}
}

