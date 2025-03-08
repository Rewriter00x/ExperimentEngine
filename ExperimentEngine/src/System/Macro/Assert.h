#pragma once

#ifdef EXP_ENABLE_ASSERTS
	#ifdef EXP_WINDOWS
		#define EXP_DEBUGBREAK() __debugbreak()
	//#elifdef EXP_MACOS
		//#define EXP_DEBUGBREAK() asm {trap}
	#endif
#else
	#define EXP_DEBUGBREAK()
#endif

#ifdef EXP_ENABLE_ASSERTS
#define EXP_ASSERT(x) if (!(x)) { EXP_LOG(Fatal, "Assertion '%s' failed at %s:%d", #x, std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__); EXP_DEBUGBREAK(); }
#define EXP_ASSERT_MSG(x, m) if (!(x)) { EXP_LOG(Fatal, "Assertion failed at %s:%d: %s", std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, m); EXP_DEBUGBREAK(); }
#else
#define EXP_ASSERT(x)
#endif
