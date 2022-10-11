#include "fm_pch.h"
#include "ServiceLocator.h"
#include "backends/renderers/directx12/DX12Renderer.h"

Renderer* ServiceLocator::m_Renderer;

Renderer* ServiceLocator::getRenderer() { return m_Renderer; }

void ServiceLocator::Provide(Renderer* renderer)
{
	m_Renderer = renderer;
}