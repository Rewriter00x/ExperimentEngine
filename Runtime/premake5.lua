project "Runtime"
	kind "ConsoleApp"
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

	debugdir (BinariesDir)

	filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

		includedirs (includeList)

	filter "system:macosx"
		defines "EXP_MACOS"

		externalincludedirs (includeList)

		-- sadly there are dynamic links so I need to link them in every executable
        links
        {
            "Cocoa.framework",
            "IOKit.framework",
            "CoreFoundation.framework",
            "QuartzCore.framework",
        }

	filter "configurations:Debug"
		defines "EXP_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "EXP_RELEASE"
		runtime "Release"
		optimize "On"