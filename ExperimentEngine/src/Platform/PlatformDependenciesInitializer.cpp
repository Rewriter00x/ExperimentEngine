#include "exppch.h"
#include "PlatformDependenciesInitializer.h"

#include "GLFW/System/ExpGLFW.h"

namespace Exp::PlatformDependenciesInitializer
{
	void Init()
	{
		ExpGLFW::Init();
	}

	void Shutdown()
	{
		ExpGLFW::Shutdown();
	}
}
