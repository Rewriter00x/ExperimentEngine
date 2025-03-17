#include "exppch.h"
#include "Application.h"

#include "Window.h"

#include "Platform/PlatformDependenciesInitializer.h"
#include "Platform/PlatformUtils.h"

namespace Exp
{
	Application::Application(const std::string& name)
	{
		EXP_ASSERT_MSG(!s_Instance, "Trying to create second instance of Application");
		s_Instance = this;

		WindowProps props;
		props.Title = name.empty() ? "Experiment Engine" : name;
		m_Window = Window::Create(props);

		m_LastFrameTime = PlatformUtils::GetTime();

		ADD_EVENT_LISTENER(this, WindowClose, OnWindowClosed);

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

	void Application::DispatchEvent(const Event& e) const
	{
		if (const auto* map = m_EventDispatcher.GetEventListeners(e.GetEventType()))
		{
			const auto it = map->find((void*)this);
			if (it != map->end())
			{
				it->second(e);
			}
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			const float time = PlatformUtils::GetTime();
			const float deltaSeconds = time - m_LastFrameTime;

			m_Window->OnUpdate(deltaSeconds);

			m_LastFrameTime = time;
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

	void Application::OnWindowClosed(const WindowCloseEvent& event)
	{
		RequestShutdown();
	}
}
