#include "exppch.h"
#include "Application.h"

#include "Engine/ImGui/ExpImGui.h"
#include "Engine/Render/RenderAPI.h"

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
		
		EXP_LOG(Log, "Application init");

		WindowProps props;
		props.Title = name.empty() ? "Experiment Engine" : name;
		m_Window = Window::Create(props);

		ExpImGui::Init();

		m_LastFrameTime = PlatformUtils::GetTime();

		ADD_EVENT_LISTENER(this, WindowClose, OnWindowClosed);
		ADD_EVENT_LISTENER(this, WindowResize, OnWindowResized);
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
		static const void* priorityList[] = { this, m_Window.get() };
		
		if (const auto* map = m_EventDispatcher.GetEventListeners(e.GetEventType()))
		{
			for (const void* obj : priorityList)
			{
				if (DispatchEventForObject(e, obj, map))
				{
					return;
				}
			}

			for (const Unique<Module>& module : m_ModuleManager.GetModules())
			{
				DispatchEventForObject(e, module.get(), map);
			}
			
			std::unordered_map<const void*, EventDispatcher::EventFn> leftMap = *map;
			for (const void* obj : priorityList)
			{
				leftMap.erase(obj);
			}

			for (const auto& [obj, func] : leftMap)
			{
				const bool res = func(e); // dispatching functions for objects is not blocking and ordered for now
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

			m_ModuleManager.OnUpdate(deltaSeconds);

			ExpImGui::BeginNewFrame();

			m_ModuleManager.OnImGuiRender();

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

	bool Application::OnWindowResized(const WindowResizeEvent& event) const
	{
		const uint32 width = event.GetWidth();
		const uint32 height = event.GetHeight();
		RenderAPI::SetViewport(0, 0, width, height);
		//s_MainCamera->SetAspectRatio((float)width / (float)height); // TODO: MOVE TO RUNTIME SINCE IT RENDERS TO VIEWPORT
		return false;
	}
}
