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

IncludeDirs = {}
IncludeDirs["GLFW"] = "%{wks.location}/ExperimentEngine/Dependencies/GLFW/GLFW/include"
IncludeDirs["glad"] = "%{wks.location}/ExperimentEngine/Dependencies/glad/glad/include"
IncludeDirs["ImGui"] = "%{wks.location}/ExperimentEngine/Dependencies/imgui/imgui"
IncludeDirs["glm"] = "%{wks.location}/ExperimentEngine/Dependencies/glm/glm"

group "Dependencies"
include "ExperimentEngine/Dependencies/GLFW"
include "ExperimentEngine/Dependencies/glad"
include "ExperimentEngine/Dependencies/imgui"
group ""

include "ExperimentEngine"
include "Laboratory"
include "Runtime"
include "Game"