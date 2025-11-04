#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core
{
	template<typename T>
	struct FunctionTraits;

	template<typename ReturnType, typename... Args>
	struct FunctionTraits<ReturnType(Args...)>
	{
		using ReturnT = ReturnType;

		template <size_t N>
		using ArgsTs = typename std::tuple_element<N, std::tuple<Args...>>::type;

		static constexpr size_t ArgsCount = sizeof...(Args);
	};

	template <typename ReturnType, typename... Args>
	struct FunctionTraits<ReturnType(*)(Args...)> : FunctionTraits<ReturnType(Args...)> {};
}