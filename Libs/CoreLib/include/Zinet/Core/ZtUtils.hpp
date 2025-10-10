#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <algorithm>
#include <iterator>

namespace zt::core
{
	inline bool Contains(auto&& container, auto&& value)
	{
		// https://github.com/shadps4-emu/shadPS4/issues/799#issuecomment-2391126439
		// "gcc 13 with full C++23 support won't be included until Debian Trixie is released."
#if ZINET_LINUX
		return std::find(container.cbegin(), container.cend(), value) != container.cend();
#else
		return std::ranges::find(container, value) != container.cend();
#endif
	}

	inline bool CompareContainers(auto&& lhs, auto&& rhs)
	{
		if (lhs.size() != rhs.size())
			return false;

		auto lhsCIt = std::cbegin(lhs);
		auto rhsCIt = std::cbegin(rhs);
		while (lhsCIt != std::cend(lhs) && rhsCIt != std::cend(rhs))
		{
			if (*lhsCIt != *rhsCIt)
				return false;

			++lhsCIt;
			++rhsCIt;
		}

		return true;
	}

	// Shouldn't be used outside the program like for saving
	template<class T>
	inline size_t GetTypeID() noexcept
	{
		// As for now typeid is good enough
		// For MSVC the hashes will be different because we are using namespaces
		auto& type = typeid(T);
		return type.hash_code();
	}
}
