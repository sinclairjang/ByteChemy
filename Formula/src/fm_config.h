#pragma once

//---- Select your backends for FM(game engine editor application)
// Default Setting on Windows: 
//	- Platform: Win32
//	- Renderer: DirectX12
#ifdef FM_PLATFORM_WINDOWS						

	#define FM_BACKENDS_PLATFORM_WIN32			// FM_BACKENDS_PLATFORMS_GLFW
	#define FM_BACKENDS_RENDERER_DIRECTX_12		// FM_BACKENDS_RENDERERS_DIRECTX_10 | FM_BACKENDS_RENDERERS_DIRECTX_11

// Default Setting on Mac: 
//	- Platform: 
//	- Renderer: 
#elif FM_PLATFORM_MAC

// Default Setting on Linux: 
//	- Platform: 
//	- Renderer:
#elif FM_PLATFORM_LINUX

#endif


//---- Define constructor and implicit cast operators to convert back-and-forth between your preferred math types and FMVecN
// This will be inlined as part of FMVecN class declarations.
#include <DirectXMath.h>						// GLM

#define DX_MATH

#ifdef DX_MATH									// GLM_MATH

#define FM_VEC2_CLASS_EXTRA																									\
	constexpr FMVec2(const DirectX::XMFLOAT2& f) : x(f.x), y(f.y) {}														\
	operator DirectX::XMFLOAT2() const { return DirectX::XMFLOAT2(x, y); }

#define FM_VEC3_CLASS_EXTRA																									\
	constexpr FMVec3(const DirectX::XMFLOAT3& f) : x(f.x), y(f.y), z(f.z) {}												\
	operator DirectX::XMFLOAT3() const { return DirectX::XMFLOAT3(x, y, z); }

#define FM_VEC4_CLASS_EXTRA																									\
	constexpr FMVec4(const DirectX::XMFLOAT4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}										\
	operator DirectX::XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, z, w); }

#define FM_MAT4_CLASS_EXTRA																									\
	constexpr FMMat4(const DirectX::XMFLOAT4X4& f)																			\
	{																														\
		m[0][0] = f.m[0][0]; m[0][1] = f.m[0][1]; m[0][2] = f.m[0][2]; m[0][3] = f.m[0][3];									\
		m[1][0] = f.m[1][0]; m[1][1] = f.m[1][1]; m[1][2] = f.m[1][2]; m[1][3] = f.m[1][3];									\
		m[2][0] = f.m[2][0]; m[2][1] = f.m[2][1]; m[2][2] = f.m[2][2]; m[2][3] = f.m[2][3];									\
		m[3][0] = f.m[3][0]; m[3][1] = f.m[3][1]; m[3][2] = f.m[3][2]; m[3][3] = f.m[3][3];									\
	}																														\
	operator DirectX::XMFLOAT4X4() const																					\
	{																														\
		return DirectX::XMFLOAT4X4(_11, _12, _13, _14,																		\
								_21, _22, _23, _24,																			\
								_31, _32, _33, _34,																			\
								_41, _42, _43, _44);																		\
	}

#define FM_PERSPECTIVE_TRANSFORM_ARGS float fovY, float aspect, float zn, float zf
#define FM_VECTOR_MULTIPLY_ADD_ARGS float d

#define FM_MAT4_STORE_HELPER																								\
	constexpr void StoreXMMatAsFMMat4(FMMat4& lv, const DirectX::XMMATRIX& rv)												\
	{																														\
		DirectX::XMFLOAT4X4 _int = (DirectX::XMFLOAT4X4)lv;																	\
		DirectX::XMStoreFloat4x4(&_int, rv);																				\
		lv = _int;																											\
	}

#define FM_VEC3_STORE_HELPER																								\
	constexpr void StoreXMVecAsFMVec3(FMVec3& lv, const DirectX::XMVECTOR& rv)												\
	{																														\
		DirectX::XMFLOAT3 _int = (DirectX::XMFLOAT3)lv;																		\
		DirectX::XMStoreFloat3(&_int, rv);																					\
		lv = _int;																											\
	}

#endif 