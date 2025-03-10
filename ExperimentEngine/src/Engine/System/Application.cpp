#include "exppch.h"
#include "Application.h"

#include "Window.h"

#include "Platform/PlatformDependenciesInitializer.h"

namespace Exp
{
	Application::Application(const std::string& name)
	{
		EXP_ASSERT_MSG(!s_Instance, "Trying to create second instance of Application");
		s_Instance = this;

		WindowProps props;
		props.Title = name.empty() ? "Experiment Engine" : name;
		m_Window = Window::Create(props);

		EXP_LOG(Log, "Application init");
	}

	Application::~Application()
	{
		EXP_LOG(Log, "Application shutdown");
	}

	void Application::RequestShutdown()
	{
        if (m_Running)
        {
            m_Running = false;
            EXP_LOG(Log, "Shutdown requested");
        }
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate(0.f);
		}
	}

	void Application::Init()
	{
		Log::Init();
		PlatformDependenciesInitializer::Init();
	}

	void Application::Shutdown()
	{
		PlatformDependenciesInitializer::Shutdown();
		Log::Shutdown();
	}
}
