#pragma once

#include "core/Renderer.h"

class DX12Renderer : Renderer
{
	virtual ~DX12Renderer() override;
	virtual void AllocateResource() override;
};

