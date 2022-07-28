#pragma once
#include "Pearl/core/Window.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);			// Windows OS system-defined callback for receiving system events

namespace Pearl 
{	
	class Win32Window : public Window 
	{
	public:
		Win32Window();
		virtual ~Win32Window()	override;

		virtual void Init(const WindowSpec* windowSpec) override;

		inline virtual unsigned int GetWidth()	const override { return windowSpec_->Width; }
		inline virtual unsigned int GetHeight() const override { return windowSpec_->Height; }

	private:
		HWND hWnd;																	// Application window handle
		WindowSpec* windowSpec_;
	};
}

