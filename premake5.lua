workspace "Pearl"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

	defines
	{
		"PEARL_BACKENDS_PLATFORMS_WIN32",
		"PEARL_BACKENDS_RENDERERS_DX_12"
	}
		
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "dependencies"
	include "Pearl/vendor/imgui"
group ""

include "Pearl"
include "Client/Sandbox"
include "Client/Main"
