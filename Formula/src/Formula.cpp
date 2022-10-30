// fm: game engine editor built on top of Dear ImGui (authored by Jinyoung Jang)
// If you want to learn more about Dear ImGui or immediate GUI, read documentation down below.
// Read online: https://github.com/ocornut/imgui

#include "fm_pch.h"

// This will provide various backends support for the editor later on.
#if defined(FM_BACKENDS_PLATFORM_WIN32) && defined(FM_BACKENDS_RENDERER_DIRECTX_12)

	#include "backends/frameworks/dx12_on_win32/main.h"

#endif
