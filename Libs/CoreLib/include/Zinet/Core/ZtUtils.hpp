#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <algorithm>

namespace zt::core
{
	inline bool Contains(const auto& container, const auto& value)
	{
		// https://github.com/shadps4-emu/shadPS4/issues/799#issuecomment-2391126439
		// "gcc 13 with full C++23 support won't be included until Debian Trixie is released."
#if ZINET_LINUX
		return std::find(container.cbegin(), container.cend(), value) != container.cend();
#else
		return std::ranges::find(container, value) != container.cend();
#endif
	}
}
