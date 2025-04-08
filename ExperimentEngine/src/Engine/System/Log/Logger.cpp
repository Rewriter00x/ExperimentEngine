#include "exppch.h"
#include "Logger.h"

#include <cstdarg>

#include "Engine/System/Application.h"

namespace Exp::Logger
{
	static const std::unordered_map<Log::LogVerbosity, int> VerbosityColorMap
	{
		{ Log::LogVerbosity::Info,		97 },
		{ Log::LogVerbosity::Log,			92 },
		{ Log::LogVerbosity::Warning,		93 },
		{ Log::LogVerbosity::Error,		91 },
		{ Log::LogVerbosity::Fatal,		31 },
	};

	static const std::string LogFileName = "Log.log";

	static FILE* LogFile;

	static inline void SetLogColor(int color) { std::cout << "\033[" << color << "m"; }
	static inline void ResetLogColor() { SetLogColor(0); }

	struct Timestamp
	{
		std::tm Tm;
		std::chrono::microseconds Ms;
	};

	static Timestamp GetTimestamp()
	{
		const auto now = std::chrono::system_clock::now();
		const auto nowMicroSeconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		const std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
		std::tm tmBuf;

#if defined(EXP_WINDOWS)
		localtime_s(&tmBuf, &timeNow);
#elif defined(EXP_MACOS)
		localtime_r(&timeNow, &tmBuf);
#endif

		return { tmBuf, nowMicroSeconds };
	}

	static void LogTimestamp(const Timestamp& ts, FILE* target)
	{
		char timeBuffer[20];
		std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &ts.Tm);

		std::fprintf(target, "[%s.%06lld] ", timeBuffer, ts.Ms.count());
	}

	void Init()
	{
		const std::filesystem::path path = g_OutputLogDirectory / LogFileName;
		LogFile = fopen(path.string().c_str(), "w");
        EXP_ASSERT(LogFile);
	}

	void Shutdown()
	{
		ResetLogColor();

		fclose(LogFile);
		LogFile = nullptr;
	}

	void LogString(Log::LogVerbosity Verbosity, const char* fmt, ...)
	{
		const Timestamp ts = GetTimestamp();
		
		va_list args;
		va_start(args, fmt);

		SetLogColor(VerbosityColorMap.at(Verbosity));
		LogTimestamp(ts, stdout);
		std::vprintf(fmt, args);
		std::printf("\n");
		ResetLogColor();
		std::fflush(stdout);

		if (Verbosity != Log::LogVerbosity::Info)
		{
			LogTimestamp(ts, LogFile);
			std::vfprintf(LogFile, fmt, args);
			std::fprintf(LogFile, "\n");
			std::fflush(LogFile);
		}

		va_end(args);

		if (Verbosity == Log::LogVerbosity::Fatal)
		{
			Application::Get().RequestShutdown();
		}
	}
}
