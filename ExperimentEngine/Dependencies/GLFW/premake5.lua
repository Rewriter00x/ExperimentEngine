project "GLFW"
    location "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir	("%{wks.location}/Binaries/" .. outputdir .. "/Dependencies/%{prj.name}")
	objdir		("%{wks.location}/Intermediate/" .. outputdir .. "/Dependencies/%{prj.name}")

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

    filter { "system:windows", "configurations:Debug" }
        runtime "Debug"
        symbols "on"

    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "on"