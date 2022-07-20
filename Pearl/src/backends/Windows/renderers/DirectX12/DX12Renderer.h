#pragma once
#include "Pearl/core/Renderer.h"

namespace Pearl
{
	class DX12Renderer : public Renderer
	{
	public:
		DX12Renderer();
		virtual ~DX12Renderer() override;

		virtual void StartUp() override { return foo; }
		virtual void ShutDown() override;
		virtual void SetRenderState(MasterConfigFile) override;
		virtual void Reset() override;

	private:
		void foo = 1;
	};
}

