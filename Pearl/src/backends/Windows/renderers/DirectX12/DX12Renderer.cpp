#include "prlpch.h"
#include "DX12Renderer.h"

namespace Pearl
{
	D3D12Renderer::D3D12Renderer(const RenderOption& renderOption) : renderOption_(renderOption) {}

	void D3D12Renderer::StartUp()
	{
		Characters chOption;
		renderOption_.QueryOptionValue("Application Mode", &chOption);

		if (chOption == "Game")
		{


		}
	};
}
	