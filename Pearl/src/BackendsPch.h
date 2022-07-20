#pragma once


//-----------------------------------------------------------------------------
// [SECTION] Renderer's Precompiled Headers
//-----------------------------------------------------------------------------

#ifdef PEARL_BACKENDS_RENDERERS_DX_12



#endif

//-----------------------------------------------------------------------------
// [SECTION] Platform's Precompiled Headers
//-----------------------------------------------------------------------------

#ifdef PEARL_BACKENDS_PLATFORMS_WIN32

	#include <windows.h>
	#include <windowsx.h> // GET_X_LPARAM(), GET_Y_LPARAM()
	#include <tchar.h>
	#include <dwmapi.h>

#endif
