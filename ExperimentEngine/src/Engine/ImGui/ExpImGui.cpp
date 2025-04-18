#include "exppch.h"
#include "ExpImGui.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Platform/PlatformUtils.h"

namespace Exp::ExpImGui
{
	static std::string s_IniPathString;
	
	void Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		const std::filesystem::path iniPath = g_OutputIniDirectory / "imgui.ini";
		s_IniPathString = iniPath.string();
		io.IniFilename = s_IniPathString.c_str();

		const float overallScale = GetOverallContentScale();

		io.Fonts->Clear();
		ImFontConfig fontConfig;
		fontConfig.SizePixels = 14.f * overallScale;
		io.Fonts->AddFontDefault(&fontConfig);

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(overallScale);

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
		
		ImGui_ImplOpenGL3_DestroyFontsTexture();
		ImGui_ImplOpenGL3_CreateFontsTexture();
	}

	void Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void BeginNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EndNewFrame()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            PlatformUtils::CacheCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            PlatformUtils::ApplyCachedContext();
        }
	}

	float GetOverallContentScale()
	{
		const glm::vec2 dpiScales = PlatformUtils::GetDPIScales();
		const float dpiScale = (dpiScales.x + dpiScales.y) * .5f;
        
		const glm::vec2 screenSize = PlatformUtils::GetScreenSize();
		const float screenHeightScale = screenSize.y / 1080.f;
        
		float overallScale = dpiScale * screenHeightScale / 2.f;
        
#if defined(EXP_MACOS)
		overallScale /= 1.25; // fine tuning for macos retina
#endif

		return overallScale;
	}
}
