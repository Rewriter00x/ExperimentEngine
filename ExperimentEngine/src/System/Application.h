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

	private:
		void Run();

		friend int ::main(int argc, char* argv[]);

	};
}