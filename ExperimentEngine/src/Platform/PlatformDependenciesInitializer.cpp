#include "exppch.h"
#include "PlatformDependenciesInitializer.h"

#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
#include "GLFW/System/ExpGLFW.h"
#endif

namespace Exp::PlatformDependenciesInitializer
{
	void Init()
	{
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
		ExpGLFW::Init();
#endif
	}

	void Shutdown()
	{
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
		ExpGLFW::Shutdown();
#endif
	}
}
