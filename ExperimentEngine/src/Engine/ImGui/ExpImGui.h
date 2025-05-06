#pragma once

struct ImGuiInputTextCallbackData;

namespace Exp
{
	namespace ExpImGui
	{
		void Init();
		void Shutdown();

		void BeginNewFrame();
		void EndNewFrame();

		float GetOverallContentScale();

		int InputTextCallback(ImGuiInputTextCallbackData* data);
	}
}
