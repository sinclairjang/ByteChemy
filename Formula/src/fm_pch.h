#pragma once
#include "fm_config.h"

//-----------------------------------------------------------------------------
// [SECTION] Platform's Precompiled Headers
//-----------------------------------------------------------------------------

#ifdef FM_BACKENDS_PLATFORM_WIN32

	#include <windows.h>
	#include <windowsx.h> // GET_X_LPARAM(), GET_Y_LPARAM()
	#include <tchar.h>
	#include <dwmapi.h>

#endif

//-----------------------------------------------------------------------------
// [SECTION] Renderer's Precompiled Headers
//-----------------------------------------------------------------------------

#ifdef FM_BACKENDS_RENDERER_DIRECTX_12
	
	#include "backends/renderers/directx12/core/d3dx12.h"

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

	using namespace Microsoft::WRL;
#endif

//-----------------------------------------------------------------------------
// [SECTION] Misc
//-----------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <stdio.h>
#include <string>
#include <numeric>
#include <memory>
#include <list>
#include <vector>
#include <deque>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <assert.h>
#include <filesystem>

#include <fbxsdk.h>
#include <entt.hpp>
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

#define BUILT_IN std::wstring() 

#define FM_ASSERT(_EXPR)            assert(_EXPR) 

#define FM_ASSERTM(exp, msg) assert(((void)msg, exp))

struct FMVec2
{
	float x, y;
	constexpr FMVec2() : x(0.0f), y(0.0f) {}
	constexpr FMVec2(float k) : x(k), y(k) {}
	constexpr FMVec2(float _x, float _y) : x(_x), y(_y) {}
	float operator[] (size_t idx) const { FM_ASSERT(idx <= 1); return (&x)[idx]; }
	float& operator[] (size_t idx) { FM_ASSERT(idx <= 1); return (&x)[idx]; }

#ifdef FM_VEC2_CLASS_EXTRA

	FM_VEC2_CLASS_EXTRA

#endif
};

struct FMVec3
{
	float x, y, z;
	constexpr FMVec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr FMVec3(float k) : x(k), y(k), z(k) {}
	constexpr FMVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	float operator[] (size_t idx) const { FM_ASSERT(idx <= 2); return (&x)[idx]; }
	float& operator[] (size_t idx) { FM_ASSERT(idx <= 2); return (&x)[idx]; }

#ifdef FM_VEC3_CLASS_EXTRA

	FM_VEC3_CLASS_EXTRA

#endif
};

struct FMVec4
{
	float x, y, z, w;
	constexpr FMVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	constexpr FMVec4(float k) : x(k), y(k), z(k), w(k) {}
	constexpr FMVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	float operator[] (size_t idx) const { FM_ASSERT(idx <= 3); return (&x)[idx]; }
	float& operator[] (size_t idx) { FM_ASSERT(idx <= 3); return (&x)[idx]; }

#ifdef FM_VEC4_CLASS_EXTRA

	FM_VEC4_CLASS_EXTRA

#endif
};

struct FMMat4
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

	FMMat4() = default;

	FMMat4(const FMMat4&) = default;
	FMMat4& operator=(const FMMat4&) = default;

	FMMat4(FMMat4&&) = default;
	FMMat4& operator=(FMMat4&&) = default;

	constexpr FMMat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
		: _11(m00), _12(m01), _13(m02), _14(m03),
		_21(m10), _22(m11), _23(m12), _24(m13),
		_31(m20), _32(m21), _33(m22), _34(m23),
		_41(m30), _42(m31), _43(m32), _44(m33) {}
	
	explicit FMMat4(const float* pArray)
		: _11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
		_21(pArray[4]),  _22(pArray[5]),  _23(pArray[6]),  _24(pArray[7]),
		_31(pArray[8]),  _32(pArray[9]),  _33(pArray[10]), _34(pArray[11]),
		_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}

	explicit FMMat4(const float k)
		: _11(k), _12(0), _13(0), _14(0),
		_21(0), _22(k), _23(0), _24(0),
		_31(0), _32(0), _33(k), _34(0),
		_41(0), _42(0), _43(0), _44(k) {}

	float  operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
	float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }

#ifdef FM_MAT4_CLASS_EXTRA

	FM_MAT4_CLASS_EXTRA

#endif
};

#ifdef FM_MAT4_STORE_HELPER
	
	FM_MAT4_STORE_HELPER

#endif

#ifdef FM_VEC3_STORE_HELPER

	FM_VEC3_STORE_HELPER

#endif

struct Vertex
{
	Vertex() {}
	Vertex(
		const FMVec3& p,
		const FMVec3& n,
		const FMVec3& t,
		const FMVec2& uv) :
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

	FMVec3 Position;
	FMVec3 Normal;
	FMVec3 TangentU;
	FMVec2 TexC;
};



struct MeshData
{
	MeshData(const std::string name)
		: Name(name) {}

	std::string Name;

	std::vector<Vertex> Vertices;
	std::vector<UINT32> Indices32;

	bool isIndices32 = false;

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
		return m_Indices16;
	}
private:
	std::vector<UINT16> m_Indices16;
};

// Example of implicit casting between FMVecN and DirectXMath math types (or any other math library of your choice for that matter)
//	void ImplicitMathTypeCastUsage()
//	{
//		// From DirectXMath to FMMath
//		DirectX::XMFLOAT4 xm4(0.0f, 0.0f, 0.0f, 0.0f);
//		FMVec4 fm4(xm4);
//	
//		// From FMMath to DirectXMath
//		DirectX::XMFLOAT4 another_xm4(fm4);
// 
//		// Container Type Cast
//		std::vector<DirectX::XMFLOAT4> xmv;
//		std::vector<FMVec4> fmv = container_cast<std::vector<FMVec4>>(xmv);
//
//		// The other direction is analogous.
//	}

template <typename To, typename From>
To container_cast(From&& from)
{
	return To(std::begin(from), std::end(from));
}

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename ...Args>
constexpr Scope<T> CreateScope(Args&& ...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename ...Args>
constexpr Ref<T> CreateRef(Args&& ...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename Key, typename T>
using HashTable = std::unordered_map<Key, T>;

std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);