#include "prlpch.h"
#include "DX12Renderer.h"

namespace Pearl
{
	D3D12Renderer::D3D12Renderer(const Option& renderOption) 
		: renderOption_(renderOption) 
	{
	}

	void D3D12Renderer::StartUp(const PlatformManager& platform)
	{
		Characters appMode;
		renderOption_.QueryOptionValue("Application Mode", &appMode);

		if (appMode == "Game")
		{

		}
		else if (appMode == "Editor")
		{

		}
	};
}