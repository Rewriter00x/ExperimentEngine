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
	
}
