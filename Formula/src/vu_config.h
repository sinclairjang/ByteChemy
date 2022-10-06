//---- Select your backends for Vu(game engine editor application)
// Default Setting on Windows: 
//	- Platform: Win32
//	- Renderer: DirectX12
#ifdef VU_PLATFORM_WINDOWS						

	#define VU_BACKENDS_PLATFORMS_WIN32			// VU_BACKENDS_PLATFORMS_GLFW
	#define VU_BACKENDS_RENDERERS_DIRECTX_12	// VU_BACKENDS_RENDERERS_DIRECTX_10 | VU_BACKENDS_RENDERERS_DIRECTX_11

// Default Setting on Mac: 
//	- Platform: 
//	- Renderer: 
#elif VU_PLATFORM_MAC

// Default Setting on Linux: 
//	- Platform: 
//	- Renderer:
#elif VU_PLATFORM_LINUX

#endif


//---- Define constructor and implicit cast operators to convert back-and-forth between your preferred math types and VuVecN
// This will be inlined as part of VuVecN class declarations.
#include <DirectXMath.h>						// GLM

#define VU_VEC2_CLASS_EXTRA																\
	constexpr VuVec2(const DirectX::XMFLOAT2& f) : x(f.x), y(f.y) {}					\
	operator DirectX::XMFLOAT2() const { return DirectX::XMFLOAT2(x, y); }

#define VU_VEC3_CLASS_EXTRA																\
	constexpr VuVec3(const DirectX::XMFLOAT3& f) : x(f.x), y(f.y), z(f.z) {}			\
	operator DirectX::XMFLOAT3() const { return DirectX::XMFLOAT3(x, y, z); }

#define VU_VEC4_CLASS_EXTRA																\
	constexpr VuVec4(const DirectX::XMFLOAT4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}	\
	operator DirectX::XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, z, w); }

#define VU_MAT4_CLASS_EXTRA																						\
	constexpr VuMat4(const DirectX::XMFLOAT4X4& f)																\
	{																											\
		m[0][0] = f.m[0][0]; m[0][1] = f.m[0][1]; m[0][2] = f.m[0][2]; m[0][3] = f.m[0][3];						\
		m[1][0] = f.m[1][0]; m[1][1] = f.m[1][1]; m[1][2] = f.m[1][2]; m[1][3] = f.m[1][3];						\
		m[2][0] = f.m[2][0]; m[2][1] = f.m[2][1]; m[2][2] = f.m[2][2]; m[2][3] = f.m[2][3];						\
		m[3][0] = f.m[3][0]; m[3][1] = f.m[3][1]; m[3][2] = f.m[3][2]; m[3][3] = f.m[3][3];						\
	}																											\
	operator DirectX::XMFLOAT4X4() const																		\
	{																											\
		return DirectX::XMFLOAT4X4(_11, _12, _13, _14,															\
								_21, _22, _23, _24,																\
								_31, _32, _33, _34,																\
								_41, _42, _43, _44);															\
	}