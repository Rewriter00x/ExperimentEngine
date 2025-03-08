#pragma once

#include "Logger.h"

namespace Exp
{
	namespace Log
	{
		void Init();
		void Shutdown();
	}
}

#define EXP_LOG(v, x, ...) ::Exp::Logger::LogString(::Exp::Log::LogVerbosity::v, x, __VA_ARGS__)