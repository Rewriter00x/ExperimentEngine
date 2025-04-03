#pragma once

// To be defined in game project
Exp::Application* CreateApplication(const std::string& name = "");

namespace Exp
{
	class Application
	{
	public:
		Application(const std::string& name = "");
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void RequestShutdown();

		inline void AddEventListener(const void* obj, EventType eventType, const EventDispatcher::EventFn& function) { m_EventDispatcher.AddEventListener(obj, eventType, function); }
		void DispatchEvent(const Event& e) const;

		inline const Unique<Window>& GetWindow() const { return m_Window; }
		
		inline const ModuleManager& GetModuleManager() const { return m_ModuleManager; }
		inline ModuleManager& GetModuleManager() { return m_ModuleManager; }

		inline static Application& Get() { return *s_Instance; }

	private:
		void Run();

		static void Init();
		static void Shutdown();

		bool OnWindowClosed(const WindowCloseEvent& event);
		bool OnWindowResized(const WindowResizeEvent& event) const;

		inline static Application* s_Instance = nullptr;

		ModuleManager m_ModuleManager;

		EventDispatcher m_EventDispatcher;

		Unique<Window> m_Window;

		float m_LastFrameTime = 0.f;

		bool m_Running = true;

		friend int ::main(int argc, char* argv[]);

	};
}