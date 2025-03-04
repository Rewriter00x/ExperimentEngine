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
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EXP_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "EXP_RELEASE"
		runtime "Release"
		optimize "On"