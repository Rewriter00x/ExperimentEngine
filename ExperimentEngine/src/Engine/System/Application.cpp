#include "exppch.h"
#include "Application.h"

namespace Exp
{
	Application::Application()
	{
		EXP_ASSERT_MSG(!s_Instance, "Trying to create second instance of Application");
		s_Instance = this;

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