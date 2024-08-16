#pragma once

#include <intrin.h>
#include <source_location>
#include <string>

namespace zt::core
{

#ifdef ZINET_DEBUG

	static inline bool Ensure(bool Value)
	{
		if (!Value)
		{
			__nop();
			__debugbreak();
		}

		return Value;
	}
#else
	static inline bool Ensure(bool Value) { return true; }
#endif

}