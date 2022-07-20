#include "prlpch.h"
#include "ServiceLocator.h"

namespace Pearl
{
	template<typename T>
	class ServiceLocator {};

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