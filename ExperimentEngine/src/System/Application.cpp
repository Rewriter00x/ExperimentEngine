#include "exppch.h"
#include "Application.h"

namespace Exp
{
	Application::Application()
	{
		Log::Init();

		EXP_LOG(Log, "Application init");
	}

	Application::~Application()
	{
		EXP_LOG(Log, "Application shutdown");

		Log::Shutdown();
	}

	void Application::Run()
	{
	}
}