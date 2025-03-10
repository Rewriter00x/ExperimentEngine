#pragma once

#ifdef EXP_ENABLE_ASSERTS
	#if defined(EXP_WINDOWS)
		#define EXP_DEBUGBREAK() __debugbreak()
	#elif defined(EXP_MACOS)
        #include <csignal>
		#define EXP_DEBUGBREAK() raise(SIGTRAP)
	#endif
#else
	#define EXP_DEBUGBREAK()
#endif

#ifdef EXP_ENABLE_ASSERTS
#define EXP_ASSERT(x) if (!(x)) { EXP_LOG(Fatal, "Assertion '%s' failed at %s:%d", #x, std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__); EXP_DEBUGBREAK(); }
#define EXP_ASSERT_MSG(x, m) if (!(x)) { EXP_LOG(Fatal, "Assertion failed at %s:%d: %s", std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, m); EXP_DEBUGBREAK(); }
#else
#define EXP_ASSERT(x)
#define EXP_ASSERT_MSG(x, m)
#endif
