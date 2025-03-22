project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir	(DependenciesBinariesDir)
	objdir		(DependenciesIntermediateDir)

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
    }

    defines
    {
        "IMGUI_DEFINE_MATH_OPERATORS",
    }

    filter "system:windows"
        systemversion "latest"

    filter { "system:windows", "configurations:Debug" }
        runtime "Debug"
        symbols "on"

    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "on"
