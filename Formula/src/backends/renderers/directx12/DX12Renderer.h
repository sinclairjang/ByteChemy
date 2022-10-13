#pragma once

#include "renderer/Renderer.h"

class DX12Renderer : Renderer
{
	virtual ~DX12Renderer() override;
	virtual void AllocateGPUResource() override;
};

