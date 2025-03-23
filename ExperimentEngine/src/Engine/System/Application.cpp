#include "exppch.h"
#include "Application.h"

#include "imgui.h"
#include "Window.h"
#include "Engine/ImGui/ExpImGui.h"

#include "Platform/PlatformDependenciesInitializer.h"
#include "Platform/PlatformUtils.h"

namespace Exp
{
	static bool DispatchEventForObject(const Event& e, const void* obj, const std::unordered_map<const void*, EventDispatcher::EventFn>* map)
	{
		const auto it = map->find(obj);
		if (it != map->end())
		{
			return it->second(e);
		}

		return false;
	}

	Application::Application(const std::string& name)
	{
		EXP_ASSERT_MSG(!s_Instance, "Trying to create second instance of Application");
		s_Instance = this;

		WindowProps props;
		props.Title = name.empty() ? "Experiment Engine" : name;
		m_Window = Window::Create(props);

		ExpImGui::Init();

		m_LastFrameTime = PlatformUtils::GetTime();

		ADD_EVENT_LISTENER(this, WindowClose, OnWindowClosed);

		EXP_LOG(Log, "Application init");
	}

	Application::~Application()
	{
		EXP_LOG(Log, "Application shutdown");

		ExpImGui::Shutdown();
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
			if (DispatchEventForObject(e, this, map)) return;
			if (DispatchEventForObject(e, m_Window.get(), map)) return;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			const float time = PlatformUtils::GetTime();
			const float deltaSeconds = time - m_LastFrameTime;

			m_Window->OnUpdate(deltaSeconds);

			ExpImGui::BeginNewFrame();

			bool open = true;
			ImGui::ShowDemoWindow(&open);

			ExpImGui::EndNewFrame();

			m_LastFrameTime = time;
		}
	}

	void Application::Init()
	{
		AssetManager::Init();
		Log::Init();
		PlatformDependenciesInitializer::Init();
	}

	void Application::Shutdown()
	{
		PlatformDependenciesInitializer::Shutdown();
		Log::Shutdown();
		AssetManager::Shutdown();
	}

	bool Application::OnWindowClosed(const WindowCloseEvent& event)
	{
		RequestShutdown();
		return true;
	}
}
