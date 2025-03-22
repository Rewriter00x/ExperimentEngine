project "ExperimentEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	(BinariesDir)
	objdir		(IntermediateDir)

    pchheader "exppch.h"
	pchsource "src/exppch.cpp"

	local localIncludes = {
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.glad}",
	}

	local includeList = table.join(CoreIncludes, localIncludes)

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
	}

	filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

		includedirs (includeList)

	filter "system:macosx"
		defines "EXP_MACOS"
		xcodebuildsettings
		{
			["GCC_PREFIX_HEADER"] = "src/exppch.h",
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