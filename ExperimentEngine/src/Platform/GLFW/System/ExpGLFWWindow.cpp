#include "exppch.h"
#include "ExpGLFWWindow.h"

#include "Engine/System/Application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Exp
{
	ExpGLFWWindow::ExpGLFWWindow(const WindowProps& props)
		: Window(props)
		, m_WindowProps(props)
	{
		EXP_LOG(Info, "Creating window %s (%d:%d)", props.Title.c_str(), props.Width, props.Height);

		m_NativeWindow = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
		EXP_ASSERT_MSG(m_NativeWindow, "Failed to create GLFW window!");

		glfwMakeContextCurrent(m_NativeWindow);

		const int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // should be done after glfwMakeContextCurrent
		EXP_ASSERT_MSG(status, "Failed to initialize GLAD");

		glfwSetWindowUserPointer(m_NativeWindow, this);

        glfwSwapInterval(1); // enable vsync

		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				WindowCloseEvent event;
				Application::Get().DispatchEvent(event);
			});
	}

	ExpGLFWWindow::~ExpGLFWWindow()
	{
		glfwDestroyWindow(m_NativeWindow);
	}

	void ExpGLFWWindow::OnUpdate(float DeltaSeconds)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 1, 1);
		glfwSwapBuffers(m_NativeWindow);
		glfwPollEvents();
	}

	void ExpGLFWWindow::SetVSync(bool enabled)
	{
		glfwMakeContextCurrent(m_NativeWindow);
		glfwSwapInterval(enabled ? 1 : 0);
	}
}
