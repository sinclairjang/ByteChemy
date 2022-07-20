project "Pearl"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "prlpch.h"
	pchsource "src/prlpch.cpp"

	files
	{
		"editor/PearlCraft/assets/fonts/**.ttf",
		"editor/PearlCraft/assets/textures/**.png",
		"editor/PearlCraft/resources/icons/**.png",
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"editor/PearlCraft/src",
		"vendor/imgui",
		"vendor/spdlog/include"
	}

	links
	{
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PEARL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PEARL_RELEASE"
		runtime "Release"
		optimize "on"
