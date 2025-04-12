workspace "ExperimentEngine"
	startproject "Laboratory"

	configurations
	{
		"Debug",
		"Release",
	}

	flags
	{
		"MultiProcessorCompile",
	}

	filter "system:windows"
		architecture "x86_64"

	filter "system:macosx"
		architecture "ARM64"

	filter ""

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

BinariesDir = 		"%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}"
IntermediateDir = 	"%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}"

DependenciesBinariesDir = 		"%{wks.location}/Binaries/" .. outputdir .. "/Dependencies/%{prj.name}"
DependenciesIntermediateDir = 	"%{wks.location}/Intermediate/" .. outputdir .. "/Dependencies/%{prj.name}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "%{wks.location}/ExperimentEngine/Dependencies/GLFW/GLFW/include"
IncludeDirs["glad"] = "%{wks.location}/ExperimentEngine/Dependencies/glad/glad/include"
IncludeDirs["ImGui"] = "%{wks.location}/ExperimentEngine/Dependencies/imgui/imgui"
IncludeDirs["glm"] = "%{wks.location}/ExperimentEngine/Dependencies/glm/glm"
IncludeDirs["stb_image"] = "%{wks.location}/ExperimentEngine/Dependencies/stb_image/stb_image"
IncludeDirs["yaml_cpp"] = "%{wks.location}/ExperimentEngine/Dependencies/yaml-cpp/yaml-cpp/include"

CoreIncludes = {
	"%{wks.location}/ExperimentEngine/src",
	"%{IncludeDirs.ImGui}",
	"%{IncludeDirs.glm}",
}

CoreDefines = {
	"YAML_CPP_STATIC_DEFINE",
}

group "Dependencies"
include "ExperimentEngine/Dependencies/GLFW"
include "ExperimentEngine/Dependencies/glad"
include "ExperimentEngine/Dependencies/imgui"
include "ExperimentEngine/Dependencies/stb_image"
include "ExperimentEngine/Dependencies/yaml-cpp"
group ""

include "ExperimentEngine"
include "Laboratory"
include "Runtime"
include "Game"