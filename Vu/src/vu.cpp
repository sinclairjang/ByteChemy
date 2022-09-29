// Vu: game engine editor built on top of Dear ImGui (authored by Jinyoung Jang)
// If you want to learn more about Dear ImGui or immediate GUI, read documentation down below.
// Read online: https://github.com/ocornut/imgui


#include "vu_pch.h"
#include "vu_config.h"

#pragma warning(disable : 4099)
#pragma warning(disable : 26812)
#pragma warning(disable : 26498)
#pragma warning(disable : 26451)

// This will provide various backends support for the editor later on.
#if defined(VU_BACKENDS_PLATFORMS_WIN32) && defined(VU_BACKENDS_RENDERERS_DIRECTX_12)
#include "frameworks/framework_win32_diretx12/main.h"
#endif
