#include "prlpch.h"
#include "PlatformManager.h"

namespace Pearl 
{
	PlatformManager::PlatformManager()
		: Window_()
	{
	}

	PlatformManager::~PlatformManager()
	{
	}

	void PlatformManager::Init(const WindowSpec* windowSpec = &WindowSpec())
	{
		Window_->Init(windowSpec);
	}
}


