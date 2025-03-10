#include "exppch.h"
#include "Window.h"

#include "Platform/GLFW/System/ExpGLFWWindow.h"

namespace Exp
{
	Window::Window(const WindowProps& props)
	{
	}

	Unique<Window> Window::Create(const WindowProps& props)
	{
#if defined(EXP_WINDOWS) || defined(EXP_MACOS)
		return MakeUnique<ExpGLFWWindow>(props);
#else
		EXP_ASSERT_MSG(false, "Unsupported platform!");
		return nullptr;
#endif
	}
}
