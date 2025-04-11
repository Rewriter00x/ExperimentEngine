project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	(DependenciesBinariesDir)
	objdir		(DependenciesIntermediateDir)

	files
	{
		"yaml-cpp/src/**.h",
		"yaml-cpp/src/**.cpp",
		
		"yaml-cpp/include/**.h",
	}

	local localIncludes = {
		"yaml-cpp/include",
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE",
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
