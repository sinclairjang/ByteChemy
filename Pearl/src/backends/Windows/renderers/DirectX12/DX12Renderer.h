#pragma once
#include "Pearl/core/Renderer.h"
#include "Pearl/core/RenderOption.h"

namespace Pearl
{
	class D3D12Renderer : public Renderer
	{
	public:
		D3D12Renderer(const RenderOption& renderOption);
		virtual ~D3D12Renderer() override;

		virtual void StartUp() override;
		virtual void CreateRenderProgram(const GameObject& gameObject) override;
		virtual void UseRenderProgram(RenderProgramID id) override;
		virtual void BindResources(const GameObject& gameObject) override;
		virtual void Render() override;
		virtual void ShutDown() override;

	private:
		
		RenderOption& renderOption_;
	};
}

