#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <algorithm>
#include <iterator>

#if ZINET_WINDOWS
#	include <windows.h>
#endif

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

	inline bool IsDebuggerAttached()
	{
#	if ZINET_WINDOWS
		return IsDebuggerPresent();
#	elif ZINET_LINUX
		assert(false); // Not implemented
#	endif
	}

	template<typename T, std::size_t N, std::size_t... Ints>
	constexpr auto MakeTupleImpl(T&& value, std::index_sequence<Ints...>)
	{
		if constexpr (std::is_reference_v<T>)
		{
			return std::forward_as_tuple(((void)Ints, std::forward<T>(value))...);
		}
		else if constexpr (std::is_object_v<T>)
		{
			return std::make_tuple(((void)Ints, std::forward<T>(value))...);
		}
		else
		{
			static_assert(false, "Well it shouldn't happen");
		}
	}

	template<typename T, std::size_t N>
	constexpr auto MakeTuple(T&& value)
	{
		return MakeTupleImpl<T, N>(std::forward<T>(value), std::make_index_sequence<N>{});
	}

	template<class Self>
	constexpr bool IsSelfConst()
	{
		return std::is_const_v<std::remove_reference_t<Self>>;
	}
}
