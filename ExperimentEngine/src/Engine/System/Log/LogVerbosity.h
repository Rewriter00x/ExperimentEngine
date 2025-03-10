#pragma once

namespace Exp
{
	namespace Log
	{
		enum class LogVerbosity : uint8
		{
			Info = 0,
			Log,
			Warning,
			Error,
			Fatal,
		};
	}
}