project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir	(DependenciesBinariesDir)
	objdir		(DependenciesIntermediateDir)

    local localIncludes = {
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.ImGui}",
	}

    files
    {
        "imgui/imconfig.h",
        "imgui/imgui.h",
        "imgui/imgui.cpp",
        "imgui/imgui_draw.cpp",
        "imgui/imgui_internal.h",
        "imgui/imgui_tables.cpp",
        "imgui/imgui_widgets.cpp",
        "imgui/imstb_rectpack.h",
        "imgui/imstb_textedit.h",
        "imgui/imstb_truetype.h",
        "imgui/imgui_demo.cpp",

        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/backends/imgui_impl_opengl3.h",
        "imgui/backends/imgui_impl_opengl3_loader.h",
        "imgui/backends/imgui_impl_glfw.cpp",
        "imgui/backends/imgui_impl_glfw.h",
    }

    defines
    {
        "IMGUI_DEFINE_MATH_OPERATORS",
    }

    filter "system:windows"
        systemversion "latest"

        includedirs (localIncludes)

    filter "system:macosx"
        externalincludedirs (localIncludes)

    filter { "system:windows", "configurations:Debug" }
        runtime "Debug"
        symbols "on"

    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "on"
