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

include "ExperimentEngine"
include "Laboratory"
include "Runtime"
include "Game"