#pragma once

struct ImGuiInputTextCallbackData;

namespace Exp
{
	namespace ExpImGui
	{
		extern const char* g_ImGuiContent_SomeFile;
		extern const char* g_ImGuiContent_IniFile;
		extern const char* g_ImGuiContent_WorldFile;
		extern const char* g_ImGuiContent_TextureFile;
    
		void Init();
		void Shutdown();

		void BeginNewFrame();
		void EndNewFrame();

		float GetOverallContentScale();

		int InputTextCallback(ImGuiInputTextCallbackData* data);
    
		const char* GetImGuiContentType(const std::filesystem::path& filepath);
	}
}
