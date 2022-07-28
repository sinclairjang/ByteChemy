#pragma once
#include "Pearl/core/Renderer.h"
#include "Pearl/core/Option.h"

namespace Pearl
{
	class PlatformManager;

	class D3D12Renderer : public Renderer
	{
	public:
		D3D12Renderer(const Option& renderOption);
		virtual ~D3D12Renderer() override;

		virtual void StartUp(const PlatformManager& platform) override;
		virtual void CreateRenderProgram(const std::fstream& shaderFile) override;
		virtual void UseRenderProgram(RenderProgramID id) override;
		virtual void BindResources(const GameObject& gameObject) override;
		virtual void Render() override;
		virtual void ShutDown() override;

	private:
		const Option& renderOption_;
	};
}

