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

	local includeList = {
		"src",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.glm}",
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