#include "ExperimentEngine.h"

#include "Modules/RuntimeModule.h"

int main(int argc, char* argv[])
{
	Exp::Application::Init();

	Exp::Application* app = CreateApplication("Runtime");
    app->GetModuleManager().AddModule<Exp::RuntimeModule>();
	app->Run();
	delete app;

	Exp::Application::Shutdown();
}
