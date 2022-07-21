#include "prlpch.h"
#include "Window.h"

#ifdef PEARL_BACKENDS_PLATFORMS_WIN32
	#include "backends/Windows/platforms/Win32/Win32Window.h"
#endif

namespace Pearl 
{
	Window* Window::Init(const WindowSpec& windowSpec) {

		#ifdef PEARL_BACKENDS_PLATFORMS_WIN32
			return new Win32Window(windowSpec);
		#else
			//TODO: Add Platforms, otherwise assert that the known platform has been specified.
		#endif
	}
}


