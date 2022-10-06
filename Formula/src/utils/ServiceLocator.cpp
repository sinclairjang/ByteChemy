#include "Vu_pch.h"
#include "ServiceLocator.h"

Renderer* ServiceLocator::getRenderer() { return m_Renderer; }

void ServiceLocator::Provide(Renderer* renderer)
{
	m_Renderer = renderer;
}