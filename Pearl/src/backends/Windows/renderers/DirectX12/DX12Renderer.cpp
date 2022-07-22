#include "prlpch.h"
#include "DX12Renderer.h"

namespace Pearl
{
	D3D12Renderer::D3D12Renderer(const RenderOption& renderOption) {}

	void D3D12Renderer::StartUp()
	{
		if (opt = gRenderOption("Game Mode"))
		{
			// ...

			if (opt = gRenderOption("Anti-Aliasing"))
			{
				auto mode = opt
			}
		}

		else // == gRenderOption("Editor Mode")
		{
			// ...
		}
}