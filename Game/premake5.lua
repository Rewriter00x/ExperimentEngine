project "Game"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	(BinariesDir)
	objdir		(IntermediateDir)

	local localIncludes = {

	}

	local includeList = table.join(CoreIncludes, localIncludes)

	local localDefines = {

	}

	local definesList = table.join(CoreDefines, localDefines)

	defines (definesList)

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	links
	{
		"ExperimentEngine",
	}

	filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

		includedirs (includeList)

	filter "system:macosx"
		defines "EXP_MACOS"

		externalincludedirs (includeList)

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