#pragma once

#include "Engine/System/Log/LogVerbosity.h"

namespace Exp
{
	namespace Logger
	{
		void Init();
		void Shutdown();

		void LogString(Log::LogVerbosity Verbosity, const char* fmt, ...);
	}
}
