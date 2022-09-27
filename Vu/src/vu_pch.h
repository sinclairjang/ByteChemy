#include "vu_config.h"

//-----------------------------------------------------------------------------
// [SECTION] Platform's Precompiled Headers
//-----------------------------------------------------------------------------

#ifdef VU_BACKENDS_PLATFORMS_WIN32

#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM(), GET_Y_LPARAM()
#include <tchar.h>
#include <dwmapi.h>

#endif

//-----------------------------------------------------------------------------
// [SECTION] Renderer's Precompiled Headers
//-----------------------------------------------------------------------------

#ifdef VU_BACKENDS_RENDERERS_DIRECTX_12

#include "frameworks/framework_win32_diretx12/d3dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <wrl.h>

#ifdef _MSC_VER
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")
#endif

#endif

//-----------------------------------------------------------------------------
// [SECTION] Misc
//-----------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <stdio.h>
#include <string>
#include <memory>
#include <list>
#include <deque>
#include <vector>
#include <map>
#include <set>
