// PearCraft: game engine editor built on top of Dear ImGui (authored by Jinyoung Jang)
// If you want to learn more about Dear ImGui or immediate GUI, read documentation down below.
// Read online: https://github.com/ocornut/imgui
#include "prlcraft_pch.h"
#include "prlcraft_config.h"

// This will provide various backends support for the editor later on.
#if defined(PEARL_BACKENDS_PLATFORMS_WIN32) && defined(PEARL_BACKENDS_RENDERERS_DIRECTX_12)
#include "frameworks/main_win32_dx12.h"
#endif
