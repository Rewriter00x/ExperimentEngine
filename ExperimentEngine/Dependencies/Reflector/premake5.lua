workspace "Reflector"
    startproject "Reflector"

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

BinariesDir = 		"%{wks.location}/../../../Binaries/" .. outputdir .. "/Dependencies/%{prj.name}"
IntermediateDir = 	"%{wks.location}/../../../Intermediate/" .. outputdir .. "/Dependencies/%{prj.name}"

IncludeDirs = {}
IncludeDirs["ImGui"] = "%{wks.location}/../../../ExperimentEngine/Dependencies/imgui/imgui"
IncludeDirs["glm"] = "%{wks.location}/../../../ExperimentEngine/Dependencies/glm/glm"
IncludeDirs["yaml_cpp"] = "%{wks.location}/../../../ExperimentEngine/Dependencies/yaml-cpp/yaml-cpp/include"

project "Reflector"
    kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	(BinariesDir)
	objdir		(IntermediateDir)

    pchheader "exppch.h"
	pchsource "../../../ExperimentEngine/src/exppch.cpp"

    local includeList = {
        "%{wks.location}/../../../ExperimentEngine/src",
        "%{IncludeDirs.ImGui}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.yaml_cpp}",
    }

    files 
    {
        "src/**.h",
        "src/**.cpp",
        "../../../ExperimentEngine/src/exppch.h",
        "../../../ExperimentEngine/src/exppch.cpp",
    }

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
    }

    filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

		includedirs (includeList)

	filter "system:macosx"
		defines "EXP_MACOS"
		xcodebuildsettings
		{
			["GCC_PREFIX_HEADER"] = "../../../ExperimentEngine/src/exppch.h",
		}

		externalincludedirs (includeList)

	filter "configurations:Debug"
		defines "EXP_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "EXP_RELEASE"
		runtime "Release"
		optimize "On"