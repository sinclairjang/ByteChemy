#pragma once

#include "core/Renderer.h"

class ServiceLocator
{
public:
	static Renderer* getRenderer();

	static void Provide(Renderer* renderer);

private:
	static Renderer* m_Renderer;
};

