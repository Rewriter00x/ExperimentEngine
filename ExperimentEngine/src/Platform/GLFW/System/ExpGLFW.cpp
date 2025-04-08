#include "exppch.h"
#include "ExpGLFW.h"

#include "GLFW/glfw3.h"

namespace Exp::ExpGLFW
{
    static GLFWwindow* s_CachedContext = nullptr;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EXP_LOG(Error, "GLFW Error (%d): %s", error, description);
	}

	void Init()
	{
		const int32 res = glfwInit();
		EXP_ASSERT_MSG(res != GLFW_FALSE, "GLFW Init failed!");
		glfwSetErrorCallback(GLFWErrorCallback);

#ifdef EXP_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	}

	void Shutdown()
	{
		glfwTerminate();
	}

	float GetTime()
	{
		return (float)glfwGetTime();
	}

    glm::vec2 GetScreenSize()
    {
#if defined(EXP_WINDOWS)
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return { (float)mode->width, (float)mode->height };
#elif defined(EXP_MACOS)
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()), &fbWidth, &fbHeight);
        return { (float)fbWidth, (float)fbHeight };
#else
		EXP_ASSERT_MSG(false, "Unsupported platform!");
		return {};
#endif
    }

    glm::vec2 GetDPIScales()
    {
        glm::vec2 scale;
        glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &scale.x, &scale.y);
        return scale;
    }

    void CacheCurrentContext()
    {
        s_CachedContext = glfwGetCurrentContext();
    }

    void ApplyCachedContext()
    {
        EXP_ASSERT_MSG(s_CachedContext, "Trying to apply null cached context!");
        glfwMakeContextCurrent(s_CachedContext);
        s_CachedContext = nullptr;
    }
}
