project "stb_image"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	(DependenciesBinariesDir)
	objdir		(DependenciesIntermediateDir)

	files
	{
		"stb_image/**.h",
		"stb_image/**.cpp",
	}

	local localIncludes = {
		"stb_image",
	}

	filter "system:windows"
		systemversion "latest"

		includedirs (localIncludes)

	filter "system:macosx"
		externalincludedirs (localIncludes)

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
