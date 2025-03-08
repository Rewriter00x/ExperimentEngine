#include "exppch.h"
#include "Log.h"

namespace Exp::Log
{
	void Init()
	{
		Logger::Init();

		EXP_LOG(Info, "Logger initialized");
	}

	void Shutdown()
	{
		Logger::Shutdown();

		EXP_LOG(Info, "Logger shutdown");
	}
}

