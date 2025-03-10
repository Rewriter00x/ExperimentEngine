project "ExperimentEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir		("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "exppch.h"
	pchsource "src/exppch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.glad}",
	}

	links
	{
		"GLFW",
		"glad",
	}

	filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

	filter "system:macosx"
		defines "EXP_MACOS"
		xcodebuildsettings
		{
			["GCC_PREFIX_HEADER"] = "src/exppch.h",
		}

	filter "configurations:Debug"
		defines "EXP_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "EXP_RELEASE"
		runtime "Release"
		optimize "On"