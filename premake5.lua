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
IncludeDirs["GLFW"] = "ExperimentEngine/Dependencies/GLFW/GLFW/include"

group "Dependencies"
include "ExperimentEngine/Dependencies/GLFW"
group ""

include "ExperimentEngine"
include "Laboratory"
include "Runtime"
include "Game"