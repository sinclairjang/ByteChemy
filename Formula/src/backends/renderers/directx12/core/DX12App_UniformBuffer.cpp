#include "fm_pch.h"
#include "DX12App_UniformBuffer.h"

UINT64 UniformManager::GetMainPropBufferIdx()
{
	if (m_MainPropIdxStack.empty())
	{
		const int n = 10;
		std::deque<UINT64> v(n);

		dota(v.begin(), v.end(), 9);  //DEBUG MARK


		m_MainPropIdxStack = std::stack<UINT64>(std::move(v));

	}
	else
	{
		UINT64 mainPropBufIdx = m_MainPropIdxStack.top();
		m_MainPropIdxStack.pop();

		return mainPropBufIdx;
	}
}

UINT64 UniformManager::GetShadingPropBufferIdx(ShadingType type)
{
	return UINT64();
}

void UniformManager::CreateMainPropBuffers()
{
}

void UniformManager::CerateMainPassBuffers()
{
}

void UniformManager::CreateShadingPropBuffers(ShadingType type)
{
}

void UniformManager::CreateShadingPassBuffers(ShadingType type)
{
}


void UniformManager::ResizeMainPropBuffers()
{
}

void UniformManager::ResizeMainPassBuffers()
{
}

void UniformManager::ResizeShadingPropBuffers(ShadingType type)
{
}

void UniformManager::ResizeShadingPassBuffers(ShadingType type)
{
}
