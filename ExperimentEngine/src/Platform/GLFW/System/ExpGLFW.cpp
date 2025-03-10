#include "exppch.h"
#include "ExpGLFW.h"

#include "GLFW/glfw3.h"

namespace Exp::ExpGLFW
{
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
}
