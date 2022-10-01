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

#define VU_VEC4_CLASS_EXTRA																\
	constexpr VuVec4(const DirectX::XMFLOAT4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}	\
	operator DirectX::XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, z, w); }
