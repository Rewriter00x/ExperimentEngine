#pragma once

// To be defined in game project
Exp::Application* CreateApplication();

namespace Exp
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		inline static Application& Get() { return *s_Instance; }

	private:
		void Run();

		inline static Application* s_Instance = nullptr;

		friend int ::main(int argc, char* argv[]);

	};
}