#include "ExperimentEngine.h"

#include "Modules/EditorModule.h"

int main(int argc, char* argv[])
{
	Exp::Application::Init();

	Exp::Application* app = CreateApplication("Laboratory");
	app->GetModuleManager().AddModule<Exp::EditorModule>();
	app->Run();
	delete app;

	Exp::Application::Shutdown();
}
