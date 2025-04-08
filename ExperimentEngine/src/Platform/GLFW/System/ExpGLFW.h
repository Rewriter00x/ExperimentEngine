#pragma once

namespace Exp
{
	namespace ExpGLFW
	{
		void Init();
		void Shutdown();

		float GetTime();
    
        glm::vec2 GetScreenSize();
        glm::vec2 GetDPIScales();
    
        void CacheCurrentContext();
        void ApplyCachedContext();
	}
};
