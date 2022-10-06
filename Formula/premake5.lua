project "Vu"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"
	
	libdirs 
	{
		"vendor/fbx/lib",
		"vendor/directxtex/lib",
	}

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Vu_pch.h"
	pchsource("src/Vu_pch.cpp")

	files
	{
		"assets/scenes/**.prl",
		"assets/shaders/**.hlsl",
		"assets/shaders/**.glsl",
		"assets/textures/**.png",

		"./src/**.h",
		"./src/**.cpp",
	}

	includedirs
	{
		".",
		"./src",
		"./vendor/imgui/",
		"./vendor/directxtex/include",
		"./vendor/entt/include",
		"./vendor/fbx/include",
		"./vendor/spdlog/include"

	}

	links
	{
		"Imgui"
	}

	filter "system:windows"
		defines "VU_PLATFORM_WINDOWS"
		systemversion "latest"
		buildoptions {"/permissive"}

	filter "configurations:Debug"
		defines "VU_DEBUG"
		runtime "Debug"
		symbols "on"
		linkoptions {"/ignore:4099"}

	filter "configurations:Release"
		defines "VU_RELEASE"
		runtime "Release"
		optimize "on"
