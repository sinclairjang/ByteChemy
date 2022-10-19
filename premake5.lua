workspace "ByteChemy"
	architecture "x64"
	startproject "Formula"

	configurations
	{
		"Debug",
		"Release"
	}
		
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "dependencies"
	include "Formula/vendor/imgui"
group ""

include "Formula"
include "Product"
