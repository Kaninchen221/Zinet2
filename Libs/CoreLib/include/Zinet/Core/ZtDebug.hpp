#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#if ZINET_MSVC
	#include <intrin.h>
#endif // ZINET_MSVC

#if ZINET_GCC
	#include <signal.h>
#endif // ZINET_GCC

namespace zt::core
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

}