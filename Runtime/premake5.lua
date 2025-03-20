project "Runtime"
	kind "ConsoleApp"
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

	local includeList = {
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