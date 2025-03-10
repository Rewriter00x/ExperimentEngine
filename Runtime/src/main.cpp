#include "ExperimentEngine.h"

int main(int argc, char* argv[])
{
	Exp::Application::Init();

	Exp::Application* app = CreateApplication("Runtime");
	app->Run();
	delete app;

	Exp::Application::Shutdown();
}