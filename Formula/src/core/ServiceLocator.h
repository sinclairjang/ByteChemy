#pragma once

#include "renderer/Renderer.h"

template <typename T>
class ServiceLocator
{
public:
	inline static T* GetService() { return m_Service;  }

	static void Provide(T* service)
	{
		m_Service = service;
	}

private:
	static T* m_Service;
};

