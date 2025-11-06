#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <tuple>

namespace zt::core
{
	template<typename T>
	struct FunctionTraits : FunctionTraits<std::remove_pointer_t<T>> {};

	template<typename ReturnType, typename... Args>
	struct FunctionTraits<ReturnType(Args...)>
	{
		using ReturnT = ReturnType;

		using TupleT = std::tuple<Args...>;

		template<size_t N>
		using ArgsTs = typename std::tuple_element_t<N, std::tuple<Args...>>;

		static constexpr size_t ArgsCount = sizeof...(Args);
	};
}