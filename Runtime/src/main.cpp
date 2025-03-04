#include "ExperimentEngine.h"

int main(int argc, char* argv[])
{
	Exp::Application* app = CreateApplication();
	app->Run();
	delete app;
}