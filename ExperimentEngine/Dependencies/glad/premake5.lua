project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir	("%{wks.location}/Binaries/" .. outputdir .. "/Dependencies/%{prj.name}")
	objdir		("%{wks.location}/Intermediate/" .. outputdir .. "/Dependencies/%{prj.name}")

    files
    {
        "glad/include/glad/glad.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/glad.c",
    }

    includedirs
    {
        "glad/include",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
