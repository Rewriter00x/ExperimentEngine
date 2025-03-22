project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir	(DependenciesBinariesDir)
	objdir		(DependenciesIntermediateDir)

    files
    {
        "GLFW/include/GLFW/glfw3.h",
        "GLFW/include/GLFW/glfw3native.h",
        "GLFW/src/internal.h",
        "GLFW/src/platform.h",
        "GLFW/src/mappings.h",
        "GLFW/src/context.c",
        "GLFW/src/init.c",
        "GLFW/src/input.c",
        "GLFW/src/monitor.c",
        "GLFW/src/platform.c",
        "GLFW/src/vulkan.c",
        "GLFW/src/window.c",
        "GLFW/src/egl_context.c",
        "GLFW/src/osmesa_context.c",
        "GLFW/src/null_platform.h",
        "GLFW/src/null_joystick.h",
        "GLFW/src/null_init.c",

        "GLFW/src/null_monitor.c",
        "GLFW/src/null_window.c",
        "GLFW/src/null_joystick.c",

    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "GLFW/src/win32_init.c",
            "GLFW/src/win32_module.c",
            "GLFW/src/win32_joystick.c",
            "GLFW/src/win32_monitor.c",
            "GLFW/src/win32_time.h",
            "GLFW/src/win32_time.c",
            "GLFW/src/win32_thread.h",
            "GLFW/src/win32_thread.c",
            "GLFW/src/win32_window.c",
            "GLFW/src/wgl_context.c",
            "GLFW/src/egl_context.c",
            "GLFW/src/osmesa_context.c",
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
        }

    filter "system:macosx"
        files
        {
            "GLFW/src/cocoa_init.m",
            "GLFW/src/cocoa_joystick.m",
            "GLFW/src/cocoa_joystick.h",
            "GLFW/src/cocoa_monitor.m",
            "GLFW/src/cocoa_time.h",
            "GLFW/src/cocoa_time.c",
            "GLFW/src/cocoa_window.m",
            "GLFW/src/posix_thread.c",
            "GLFW/src/posix_thread.h",
            "GLFW/src/posix_time.c",
            "GLFW/src/posix_time.h",
            "GLFW/src/posix_module.c",
            "GLFW/src/nsgl_context.m",
            "GLFW/src/egl_context.c",
            "GLFW/src/osmesa_context.c",
        }

        -- sadly there are dynamic links so I need to link them in every executable
        links
        {
            "Cocoa.framework",
            "IOKit.framework",
            "CoreFoundation.framework",
            "QuartzCore.framework",
        }

        defines
        {
            "_GLFW_COCOA",
        }

    filter { "system:macosx", "files:**.m" }
        buildoptions { "-x objective-c" }


    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"