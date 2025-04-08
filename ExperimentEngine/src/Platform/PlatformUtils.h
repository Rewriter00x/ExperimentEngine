#pragma once

namespace Exp
{
	namespace PlatformUtils
	{
		float GetTime();
    
        glm::vec2 GetScreenSize();
        glm::vec2 GetDPIScales();
    
        void CacheCurrentContext();
        void ApplyCachedContext();
	}
}
