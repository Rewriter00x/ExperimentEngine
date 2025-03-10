workspace "ExperimentEngine"
	architecture "x86_64"
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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "%{wks.location}/ExperimentEngine/Dependencies/GLFW/GLFW/include"
IncludeDirs["glad"] = "%{wks.location}/ExperimentEngine/Dependencies/glad/glad/include"

group "Dependencies"
include "ExperimentEngine/Dependencies/GLFW"
include "ExperimentEngine/Dependencies/glad"
group ""

include "ExperimentEngine"
include "Laboratory"
include "Runtime"
include "Game"