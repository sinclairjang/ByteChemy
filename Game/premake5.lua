project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"assets/scenes/**.prl",
		"assets/shaders/**.hlsl",
		"assets/shaders/**.glsl",
		"assets/textures/**.png",

		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Game/src"
	}

	links
	{
		
	}

	filter "system:windows"
		defines "PEARL_PLATFORM_WINDOWS"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PEARL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PEARL_Release"
		runtime "Release"
		optimize "on"
