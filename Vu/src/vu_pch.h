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
	
	#include "backends/renderers/directx12/utils/d3dx12.h"

	#include <d3d12.h>
	#include <dxgi1_4.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	#include <DirectXPackedVector.h>
	#include <DirectXColors.h>
	#include <wrl.h>
	#include "backends/renderers/directx12/utils/d3dx12_helperbuf.h"

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

#include <fbxsdk.h>

#include <DirectXTex.h>
#include <DirectXTex.inl>

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

struct VuVec2
{
	float x, y;
	constexpr VuVec2() : x(0.0f), y(0.0f) {}
	constexpr VuVec2(float k) : x(k), y(k) {}
	constexpr VuVec2(float _x, float _y) : x(_x), y(_y) {}
	float operator[] (size_t idx) const { VU_ASSERT(idx <= 1); return (&x)[idx]; }
	float& operator[] (size_t idx) { VU_ASSERT(idx <= 1); return (&x)[idx]; }

#ifdef VU_VEC2_CLASS_EXTRA

	VU_VEC2_CLASS_EXTRA

#endif
};

struct VuVec3
{
	float x, y, z;
	constexpr VuVec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr VuVec3(float k) : x(k), y(k), z(k) {}
	constexpr VuVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	float operator[] (size_t idx) const { VU_ASSERT(idx <= 2); return (&x)[idx]; }
	float& operator[] (size_t idx) { VU_ASSERT(idx <= 2); return (&x)[idx]; }

#ifdef VU_VEC3_CLASS_EXTRA

	VU_VEC3_CLASS_EXTRA

#endif
};

struct VuVec4
{
	float x, y, z, w;
	constexpr VuVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	constexpr VuVec4(float k) : x(k), y(k), z(k), w(k) {}
	constexpr VuVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	float operator[] (size_t idx) const { VU_ASSERT(idx <= 3); return (&x)[idx]; }
	float& operator[] (size_t idx) { VU_ASSERT(idx <= 3); return (&x)[idx]; }

#ifdef VU_VEC4_CLASS_EXTRA

	VU_VEC4_CLASS_EXTRA

#endif
};

struct VuMat4
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	VuMat4() = default;

	VuMat4(const VuMat4&) = default;
	VuMat4& operator=(const VuMat4&) = default;

	VuMat4(VuMat4&&) = default;
	VuMat4& operator=(VuMat4&&) = default;

	constexpr VuMat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
		: _11(m00), _12(m01), _13(m02), _14(m03),
		_21(m10), _22(m11), _23(m12), _24(m13),
		_31(m20), _32(m21), _33(m22), _34(m23),
		_41(m30), _42(m31), _43(m32), _44(m33) {}
	
	explicit VuMat4(_In_reads_(16) const float* pArray)
		: _11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
		_21(pArray[4]),  _22(pArray[5]),  _23(pArray[6]),  _24(pArray[7]),
		_31(pArray[8]),  _32(pArray[9]),  _33(pArray[10]), _34(pArray[11]),
		_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}

	explicit VuMat4(_In_reads_(1) const float k)
		: _11(k), _12(0), _13(0), _14(0),
		_21(0), _22(k), _23(0), _24(0),
		_31(0), _32(0), _33(k), _34(0),
		_41(0), _42(0), _43(0), _44(k) {}

	float  operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
	float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }

#ifdef VU_MAT4_CLASS_EXTRA

	VU_MAT4_CLASS_EXTRA

#endif
};

struct Vertex
{
	Vertex() {}
	Vertex(
		const VuVec3& p,
		const VuVec3& n,
		const VuVec3& t,
		const VuVec2& uv) :
		Position(p),
		Normal(n),
		TangentU(t),
		TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TangentU(tx, ty, tz),
		TexC(u, v) {}

	VuVec3 Position;
	VuVec3 Normal;
	VuVec3 TangentU;
	VuVec2 TexC;
};

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<UINT32> Indices32;

	std::vector<UINT16>& GetIndices16()
	{
		if (m_Indices16.empty())
		{
			m_Indices16.resize(Indices32.size());
			for (size_t i = 0; i < m_Indices16.size(); ++i)
			{
				m_Indices16[i] = static_cast<UINT16>(Indices32[i]);
			}
		}

	}
private:
	std::vector<UINT16> m_Indices16;
};