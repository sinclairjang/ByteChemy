#include "Vu_config.h"

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
	
	#include "backends/renderers/directx12/d3dx12.h"
	#include <d3d12.h>
	#include <dxgi1_4.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	#include <DirectXPackedVector.h>
	#include <DirectXColors.h>
	#include <wrl.h>

	#pragma comment(lib, "d3d12")
	#pragma comment(lib, "dxgi")
	#pragma comment(lib, "d3dcompiler")

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
#include <assert.h>

#include "fbxsdk.h"

#include "DirectXTex.h"
#include "DirectXTex.inl"

#ifdef _DEBUG

	#pragma comment(lib, "x64/debug/libfbxsdk-mt")
	#pragma comment(lib, "x64/debug/libxml2-mt")
	#pragma comment(lib, "x64/debug/zlib-mt")
	
	#pragma comment(lib, "x64/debug/DirectXTex")

#else

	#pragma comment(lib, "x64/release/libfbxsdk-mt")
	#pragma comment(lib, "x64/release/libxml2-mt")
	#pragma comment(lib, "x64/release/zlib-mt")

	#pragma comment(lib, "x64/release/DirectXTex")

#endif

#define VU_ASSERT(_EXPR)            assert(_EXPR) 

struct VuVec4
{
	float x, y, z, w;
	constexpr VuVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	constexpr VuVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	float operator[] (size_t idx) const { VU_ASSERT(idx <= 3); return (&x)[idx]; }
	float& operator[] (size_t idx) { VU_ASSERT(idx <= 3); return (&x)[idx]; }

#ifdef VU_VEC4_CLASS_EXTRA

	VU_VEC4_CLASS_EXTRA

#endif
};
