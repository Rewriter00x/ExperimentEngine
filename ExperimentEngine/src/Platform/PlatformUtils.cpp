#include "exppch.h"
#include "PlatformUtils.h"

#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
#include "GLFW/System/ExpGLFW.h"
#endif

namespace Exp::PlatformUtils
{
	float GetTime()
	{
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
		return ExpGLFW::GetTime();
#else
		EXP_ASSERT_MSG(false, "Unsupported platform!");
		return 0.f;
#endif
	}

    glm::vec2 GetScreenSize()
    {
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
        return ExpGLFW::GetScreenSize();
#else
        EXP_ASSERT_MSG(false, "Unsupported platform!");
        return {};
#endif
    }

    glm::vec2 GetDPIScales()
    {
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
        return ExpGLFW::GetDPIScales();
#else
        EXP_ASSERT_MSG(false, "Unsupported platform!");
        return {};
#endif
    }

    void CacheCurrentContext()
    {
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
        ExpGLFW::CacheCurrentContext();
#else
        EXP_ASSERT_MSG(false, "Unsupported platform!");
#endif
    }

    void ApplyCachedContext()
    {
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
        ExpGLFW::ApplyCachedContext();
#else
        EXP_ASSERT_MSG(false, "Unsupported platform!");
#endif
    }
	
}
