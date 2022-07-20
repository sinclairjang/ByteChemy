#pragma once

#include "Pearl/core/Window.h"

namespace Pearl
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
}


