#pragma once

#include "ServiceLocator.h"
#include "Window.h"

namespace Pearl 
{
	struct WindowSpec {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSpec(const std::string& title = "Pearl Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class PlatformManager 
	{
		friend ServiceLocator<Window>;

	public:
		PlatformManager();
		PlatformManager(const PlatformManager& rhs) = delete;
		PlatformManager& operator=(const PlatformManager& rhs) = delete;
		~PlatformManager();
		
		void Init(const WindowSpec* windowSpec = &WindowSpec()); // Create an application window managed by OS

	private:
		std::unique_ptr<Window> Window_; // Service API to be set by ServiceLocator
	};
}




