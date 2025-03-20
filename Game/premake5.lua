project "Game"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir		("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/ExperimentEngine/src",
		"%{IncludeDirs.glm}",
	}

	links
	{
		"ExperimentEngine",
	}

	filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

	filter "system:macosx"
		defines "EXP_MACOS"

	filter "configurations:Debug"
		defines "EXP_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "EXP_RELEASE"
		runtime "Release"
		optimize "On"


project "Laboratory"

	links
	{
		"Game",
	}

project "Runtime"

	links
	{
		"Game",
	}