#include "exppch.h"
#include "ExpGLFWWindow.h"

#include "Engine/Render/RenderAPI.h"
#include "Engine/Render/Renderer.h"
#include "Engine/System/Application.h"

#include "GLFW/glfw3.h"

namespace Exp
{
	ExpGLFWWindow::ExpGLFWWindow(const WindowProps& props)
		: Window(props)
		, m_WindowProps(props)
	{
		EXP_LOG(Log, "Creating window %s (%d:%d)", props.Title.c_str(), props.Width, props.Height);
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_NativeWindow = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMaximizeWindow(m_NativeWindow);
		EXP_ASSERT_MSG(m_NativeWindow, "Failed to create GLFW window!");

		glfwMakeContextCurrent(m_NativeWindow);

		RenderAPI::Init((const void*)glfwGetProcAddress); // should be done after glfwMakeContextCurrent
		Renderer::Init();

		glfwSetWindowUserPointer(m_NativeWindow, this);

        glfwSwapInterval(1); // enable vsync

		InitEvents();
	}

	ExpGLFWWindow::~ExpGLFWWindow()
	{
		Renderer::Shutdown();
		RenderAPI::Shutdown();
		
		glfwDestroyWindow(m_NativeWindow);
	}

	void ExpGLFWWindow::OnUpdate(float DeltaSeconds)
	{
		glfwSwapBuffers(m_NativeWindow);
		glfwPollEvents();

		RenderAPI::Clear();
		RenderAPI::SetClearColor({ 1, 0, 1, 1 });
	}

	void ExpGLFWWindow::SetVSync(bool enabled)
	{
		glfwMakeContextCurrent(m_NativeWindow);
		glfwSwapInterval(enabled ? 1 : 0);
	}

	void ExpGLFWWindow::InitEvents() const
	{
		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				const WindowCloseEvent event;
				Application::Get().DispatchEvent(event);
			});

		glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));
				expWindow->m_WindowProps.Width = width;
				expWindow->m_WindowProps.Height = height;

				const WindowResizeEvent event(width, height);
				Application::Get().DispatchEvent(event);
			});

		glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
					case GLFW_PRESS:
					{
						const KeyPressedEvent event(static_cast<KeyCode>(key), false);
						Application::Get().DispatchEvent(event);
						break;
					}

					case GLFW_RELEASE:
					{
						const KeyReleasedEvent event(static_cast<KeyCode>(key));
						Application::Get().DispatchEvent(event);
						break;
					}

					case GLFW_REPEAT:
					{
						const KeyPressedEvent event(static_cast<KeyCode>(key), true);
						Application::Get().DispatchEvent(event);
						break;
					}

					default:;
				}
			});

		glfwSetCharCallback(m_NativeWindow, [](GLFWwindow* window, unsigned int keycode)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				const KeyTypedEvent event(static_cast<KeyCode>(keycode));
				Application::Get().DispatchEvent(event);
			});

		glfwSetMouseButtonCallback(m_NativeWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
					case GLFW_PRESS:
					{
						const MouseButtonPressedEvent event(static_cast<MouseCode>(button));
						Application::Get().DispatchEvent(event);
						break;
					}

					case GLFW_RELEASE:
					{
						const MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
						Application::Get().DispatchEvent(event);
						break;
					}

					default:;
				}
			});

		glfwSetScrollCallback(m_NativeWindow, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				const MouseScrolledEvent event((float)xOffset, (float)yOffset);
				Application::Get().DispatchEvent(event);
			});

		glfwSetCursorPosCallback(m_NativeWindow, [](GLFWwindow* window, double xPos, double yPos)
			{
				ExpGLFWWindow* expWindow = static_cast<ExpGLFWWindow*>(glfwGetWindowUserPointer(window));

				const MouseMovedEvent event((float)xPos, (float)yPos);
				Application::Get().DispatchEvent(event);
			});
	}
}
