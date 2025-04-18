#pragma once

namespace Exp
{
	namespace ExpImGui
	{
		void Init();
		void Shutdown();

		void BeginNewFrame();
		void EndNewFrame();

		float GetOverallContentScale();
	}
}
