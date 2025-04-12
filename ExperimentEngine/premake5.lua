project "ExperimentEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir	(BinariesDir)
	objdir		(IntermediateDir)

	dependson
	{
		"Reflector",
	}

    pchheader "exppch.h"
	pchsource "src/exppch.cpp"

	local localIncludes = {
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.stb_image}",
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
		"Reflector",
		"GLFW",
		"glad",
		"ImGui",
		"stb_image",
		"yaml-cpp",
	}

	libdirs
	{
		DependenciesBinariesDir .. "/../Reflector",
	}

	filter "system:windows"
		defines "EXP_WINDOWS"
		systemversion "latest"

		prebuildcommands
		{
			"cd ../ExperimentEngine/Dependencies/Reflector/script/",
			"python3 reflector.py",
			"cd ../",
			"call GenerateProjectFiles.bat",
			"msbuild Reflector.sln /p:Configuration=%{cfg.buildcfg}",
		}

		includedirs (includeList)

	filter "system:macosx"
		defines "EXP_MACOS"
		xcodebuildsettings
		{
			["GCC_PREFIX_HEADER"] = "src/exppch.h",
		}

		prebuildcommands
		{
			"cd ../ExperimentEngine/Dependencies/Reflector/script",
			"python3 reflector.py",
			"cd ../",
			"sh GenerateProjectFiles",
			"xcodebuild -workspace Reflector.xcworkspace -scheme Reflector -configuration %{cfg.buildcfg} -quiet",
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