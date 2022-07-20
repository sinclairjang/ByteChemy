#pragma once
#include "backends/Windows/renderers/DirectX12/DX12Renderer.h"

namespace Pearl
{
	template<typename T>
	class ServiceLocator;

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



