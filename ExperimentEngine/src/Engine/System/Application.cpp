#include "exppch.h"
#include "Application.h"

#include "imgui.h"
#include "Window.h"
#include "Engine/ImGui/ExpImGui.h"
#include "Engine/Render/EditorCamera.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/RenderAPI.h"

#include "Engine/Render/RenderData/Framebuffer.h"
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
	static Framebuffer* s_Framebuffer;
	static glm::vec2 s_ViewportSize = { 0.f, 0.f };

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
		s_MainCamera->SetPosition({ 0.f, 0.f, 10.f });

		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::COLOR, FramebufferTextureFormat::DEPTH };
		spec.Width = props.Width;
		spec.Height = props.Height;
		s_ViewportSize = { props.Width, props.Height };
		s_Framebuffer = new Framebuffer(spec);

		ADD_EVENT_LISTENER(this, WindowClose, OnWindowClosed);
		ADD_EVENT_LISTENER(this, WindowResize, OnWindowResized);
	}

	Application::~Application()
	{
		EXP_LOG(Log, "Application shutdown");

		delete s_Framebuffer;
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

			static glm::vec3 position = { 0.f, 0.f, 0.f };
			static glm::vec3 rotation = { 0.f, 0.f, 0.f };
			static glm::vec2 size = { 1.f, 1.f };
			static glm::vec4 color = { 1.f, 0.f, 0.f, 1.f };
			static Shared<Texture> texture = MakeShared<Texture>(g_EngineResourcesDirectory / "Textures" / "CheckerBoard.png");
			static int32 attachmentIndex = 0;

			const FramebufferSpecification& spec = s_Framebuffer->GetSpecification();
			if (s_ViewportSize.x > 0.0f && s_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.Width != s_ViewportSize.x || spec.Height != s_ViewportSize.y))
			{
				s_Framebuffer->Resize((uint32)s_ViewportSize.x, (uint32)s_ViewportSize.y);

				s_MainCamera->SetAspectRatio(s_ViewportSize.x / s_ViewportSize.y);
			}

			s_Framebuffer->Bind();

			RenderAPI::Clear();
			RenderAPI::SetClearColor({ 1, 0, 1, 1 });

			Renderer::BeginBatch(*s_MainCamera);
			
			for (int32 i = 0; i < 25; i++)
			{
				const int32 div = i / 5;
				const int32 mod = i % 5;
				glm::vec3 pos = position;
				constexpr float sqrt = 1.4142135f;
				pos += glm::vec3 { sqrt * size.x * mod, -sqrt * size.y * div, 0.f };
				Renderer::DrawQuad({ pos, rotation, size, color, texture });
			}
			
			Renderer::EndBatch();

			Framebuffer::Unbind();

			ExpImGui::BeginNewFrame();

			ImGui::Begin("Test");
			ImGui::DragFloat3("Position", glm::value_ptr(position), .01f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 1.f);
			ImGui::DragFloat2("Size", glm::value_ptr(size), .01f);
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
			ImGui::DragInt("Attachment Index", &attachmentIndex, 1.f, 0, 1);
			ImGui::End();

			ImGui::Begin("Viewport");
			const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			s_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			const uint32 textureID = s_Framebuffer->GetAttachmentRendererID(attachmentIndex);
			ImGui::Image(textureID, viewportPanelSize, { 0.f, 1.f }, { 1.f, 0.f });
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
		//s_MainCamera->SetAspectRatio((float)width / (float)height);
		return false;
	}
}
