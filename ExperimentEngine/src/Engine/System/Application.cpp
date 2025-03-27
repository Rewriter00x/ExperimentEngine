#include "exppch.h"
#include "Application.h"

#include "imgui.h"
#include "Window.h"
#include "Engine/ImGui/ExpImGui.h"
#include "Engine/Render/EditorCamera.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/RenderAPI.h"
#include "Engine/Render/RenderData/Texture.h"

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

	static EditorCamera* s_MainCamera; // TODO move

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

		s_MainCamera = new EditorCamera;

		ADD_EVENT_LISTENER(this, WindowClose, OnWindowClosed);
		ADD_EVENT_LISTENER(this, WindowResize, OnWindowResized);
	}

	Application::~Application()
	{
		EXP_LOG(Log, "Application shutdown");

		delete s_MainCamera;

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

			s_MainCamera->OnUpdate(deltaSeconds);

			m_Window->OnUpdate(deltaSeconds);

			static glm::vec3 position = { 0.f, 0.f, -10.f };
			static glm::vec2 size = { 1.f, 1.f };
			static glm::vec4 color = { 1.f, 0.f, 0.f, 1.f };
			static Shared<Texture> texture = MakeShared<Texture>(g_EngineResourcesDirectory / "Textures" / "CheckerBoard.png");

			Renderer::BeginBatch(*s_MainCamera);
			
			Renderer::DrawQuad(position, size, color);

			const glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
				* glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });

			Renderer::DrawQuad(transform, texture);
			
			Renderer::EndBatch();

			ExpImGui::BeginNewFrame();

			ImGui::Begin("Test");
			ImGui::DragFloat3("Position", glm::value_ptr(position), .01f);
			ImGui::DragFloat2("Size", glm::value_ptr(size), .01f);
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
			ImGui::End();

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
		s_MainCamera->SetAspectRatio((float)width / (float)height);
		return false;
	}
}
