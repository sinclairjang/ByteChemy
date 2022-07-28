#pragma once

#include "backends/Windows/platforms/Win32/Win32Window.h"
#include "backends/Windows/renderers/DirectX12/DX12Renderer.h"

namespace Pearl
{
	template<typename T>
	class ServiceLocator;

	template<>
	class ServiceLocator<Window>
	{
	public:
		static Window* getWindow();
		static void provide(Window* service);

	private:
		static Window* service_;
	};

	template<>
	class ServiceLocator<Renderer>
	{
	public:
		static Renderer* getRenderer();
		static void provide(Renderer* service);

	private:
		static Renderer* service_;
	};
}



