#include "prlpch.h"
#include "ServiceLocator.h"

namespace Pearl
{
	template<typename T>
	class ServiceLocator {};

	template<>
	class ServiceLocator<Window>
	{
	public:
		static Window* getWindow() { return service_; }

		static void provide(Window* service)
		{
			service_ = service;
		}

	private:
		static Window* service_;
	};

	template<>
	class ServiceLocator<Renderer>
	{
	public:
		static Renderer* getRenderer() { return service_; }

		static void provide(Renderer* service)
		{
			service_ = service;
		}

	private:
		static Renderer* service_;
	};
}