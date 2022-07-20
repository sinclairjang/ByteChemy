#pragma once
#include "Pearl/core/Window.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);			// Windows OS system-defined callback for receiving system events

namespace Pearl 
{	
	class Win32Window : public Window 
	{
	public:
		Win32Window(const WindowSpec& windowSpec);
		virtual ~Win32Window()	override;

		//TODO: virtual void OnUpdate() override;

		inline virtual unsigned int GetWidth()	const override { return m_WindowSpec.Width; }
		inline virtual unsigned int GetHeight() const override { return m_WindowSpec.Height; }

	private:
		HWND hWnd;																	// Application window handle
		WindowSpec m_WindowSpec;
	};
}

