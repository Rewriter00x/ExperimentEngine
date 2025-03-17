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

		inline static Application& Get() { return *s_Instance; }

	private:
		void Run();

		static void Init();
		static void Shutdown();

		inline static Application* s_Instance = nullptr;

		Unique<Window> m_Window;

		float m_LastFrameTime = 0.f;

		bool m_Running = true;

		friend int ::main(int argc, char* argv[]);

	};
}