#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#ifdef ZINET_MSVC
	#include <intrin.h>
#elif ZINET_GCC
	#include <signal.h>
#endif

namespace zt::core
{

#ifdef ZINET_DEBUG

	static inline bool Ensure(bool Value)
	{
#ifdef ZINET_MSVC
		if (!Value)
		{
			__nop();
			__debugbreak();
		}
#elif ZINET_GCC
		raise(SIGTRAP);
#endif

		return Value;
	}
#else
	static inline bool Ensure(bool Value) { return true; }
#endif

}