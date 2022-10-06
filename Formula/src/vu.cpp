// Vu: game engine editor built on top of Dear ImGui (authored by Jinyoung Jang)
// If you want to learn more about Dear ImGui or immediate GUI, read documentation down below.
// Read online: https://github.com/ocornut/imgui

#include "Vu_pch.h"
#include "Vu_config.h"

// This will provide various backends support for the editor later on.
#if defined(VU_BACKENDS_PLATFORMS_WIN32) && defined(VU_BACKENDS_RENDERERS_DIRECTX_12)

	#include "backends/frameworks/dx12_on_win32/main.h"

#endif