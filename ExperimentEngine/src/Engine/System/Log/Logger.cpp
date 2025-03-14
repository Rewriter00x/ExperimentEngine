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

	static const std::filesystem::path LogFilePath = "Log";
	static const std::string LogFileName = "Log.log"; // TODO: use asset manager

	static FILE* LogFile;

	static inline void SetLogColor(int color) { std::cout << "\033[" << color << "m"; }
	static inline void ResetLogColor() { SetLogColor(0); }

	void Init()
	{
		std::filesystem::path path = LogFilePath;
		std::filesystem::create_directories(path);
		path /= LogFileName;
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
		va_list args;
		va_start(args, fmt);

		SetLogColor(VerbosityColorMap.at(Verbosity));
		std::vprintf(fmt, args);
		std::printf("\n");
		ResetLogColor();

		if (Verbosity != Log::LogVerbosity::Info)
		{
			std::vfprintf(LogFile, fmt, args);
			std::fprintf(LogFile, "\n");
		}

		va_end(args);

		if (Verbosity == Log::LogVerbosity::Fatal)
		{
			Application::Get().RequestShutdown();
		}
	}
}
