#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <exception>

#if ZINET_MSVC
	#include <intrin.h>
#endif // ZINET_MSVC

#if ZINET_GCC
	#include <signal.h>
#endif // ZINET_GCC

/// Not in "core" namespace because used too often
namespace zt
{

#if ZINET_DEBUG

	static inline bool Ensure(bool Value)
	{
		if (!Value)
		{
		#if ZINET_MSVC
			__nop();
			__debugbreak();
		#endif // ZINET_MSVC

		#if ZINET_GCC
			raise(SIGTRAP);
		#endif // ZINET_GCC
		}

		return Value;
	}
#else
	static inline bool Ensure(bool Value) { return Value; }
#endif

	inline static void TerminateDebug() noexcept
	{
#if ZINET_DEBUG
		std::terminate();
#endif
	}

}