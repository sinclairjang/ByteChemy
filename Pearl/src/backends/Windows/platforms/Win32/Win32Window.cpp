#include "prlpch.h"
#include "Win32Window.h"
#include "backends/imgui_impl_win32.h"

namespace Pearl 
{
	Win32Window::Win32Window()
		: windowSpec_(nullptr)
	{
	}

	Win32Window::~Win32Window()
	{
	}

	void Win32Window::Init(const WindowSpec* windowSpec)
	{
		*windowSpec_ = *windowSpec;
	}
}

{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, (const TCHAR*)m_WindowSpec.Title.c_str(), NULL };
	::RegisterClassEx(&wc);
	hWnd = ::CreateWindow(wc.lpszClassName, _T("Pearl Engine"), WS_OVERLAPPEDWINDOW, 100, 100, m_WindowSpec.Width, m_WindowSpec.Height, NULL, NULL, wc.hInstance, NULL);
}
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImPlWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImPlWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_SIZE:
			//TODO: Resize swap chain buffers
			return 0;
	
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU)	// Disable the menu activation via the ALT key
				return 0;
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}