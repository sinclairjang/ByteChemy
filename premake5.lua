workspace "Pearl"
	architecture "x64"
	startproject "PearlCraft"

	configurations
	{
		"Debug",
		"Release"
	}
		
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "dependencies"
	include "vendor/imgui"
group ""

include "PearlCraft"
include "Game"
