#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core
{
	template<typename T>
	struct FunctionTraits;

	template<typename Return, typename... Args>
	struct FunctionTraits<Return(Args...)>
	{
		using ReturnT = Return;

		template <std::size_t N>
		using ArgsTs = typename std::tuple_element<N, std::tuple<Args...>>::type;

		static constexpr size_t ArgsCount = sizeof...(Args);
	};
}