workspace "Vu"
	architecture "x64"
	startproject "Vu"

	configurations
	{
		"Debug",
		"Release"
	}
		
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "dependencies"
	include "vendor/imgui"
group ""

include "Vu"
include "Game"
