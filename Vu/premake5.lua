project "Vu"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"
	
	libdirs {"../vendor/fbx/lib"}

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vu_pch.h"
	pchsource("src/vu_pch.cpp")

	files
	{
		"assets/scenes/**.prl",
		"assets/shaders/**.hlsl",
		"assets/shaders/**.glsl",
		"assets/textures/**.png",

		"./**.h",
		"./**.cpp",
	}

	includedirs
	{
		".",
		"./src",
		"../vendor/imgui",
		"../vendor/entt/include",
		"../vendor/fbx/include"
	}

	links
	{
		"Imgui"
	}

	filter "system:windows"
		defines "VU_PLATFORM_WINDOWS"
		systemversion "latest"

	filter "configurations:Debug"
		defines "VU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VU_RELEASE"
		runtime "Release"
		optimize "on"