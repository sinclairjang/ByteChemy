project "Formula"
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

	pchheader "fm_pch.h"
	pchsource("src/fm_pch.cpp")

	disablewarnings 
	{
		"4099",
		"6285",
		"6387",
		"26437",
		"26451",
		"26498",
		"26812",
		"26495",
		"26800",

	}

	files
	{
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
		defines "FM_PLATFORM_WINDOWS"
		systemversion "latest"
		buildoptions {"/permissive"}

	filter "configurations:Debug"
		defines "FM_DEBUG"
		runtime "Debug"
		symbols "on"
		linkoptions {"/ignore:4099"}

	filter "configurations:Release"
		defines "FM_RELEASE"
		runtime "Release"
		optimize "on"
