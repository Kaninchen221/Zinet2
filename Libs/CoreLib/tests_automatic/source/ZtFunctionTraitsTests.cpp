#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtFunctionTraits.hpp"

namespace zt::core::tests
{
	class FunctionTraitsTests : public ::testing::Test
	{
	protected:

		static int32_t ExampleFunction(float, const int64_t&) { return {}; }
	};

	TEST_F(FunctionTraitsTests, Test)
	{
		using Traits = FunctionTraits<decltype(ExampleFunction)>;

		static_assert(std::is_same_v<Traits::ReturnT, int32_t>);
		static_assert(std::is_same_v<Traits::ArgsTs<0>, float>);
		static_assert(std::is_same_v<Traits::ArgsTs<1>, const int64_t&>);
		static_assert(std::is_same_v<Traits::TupleT, std::tuple<float, const int64_t&>>);

		[[maybe_unused]]
		Traits::ReturnT value = ExampleFunction(Traits::ArgsTs<0>{}, Traits::ArgsTs<1>{});
	}

	TEST_F(FunctionTraitsTests, TestFuncAsParam)
	{
		auto testFunc = [](auto func)
		{
			using Traits = FunctionTraits<decltype(func)>;

			// That was enough to invoke the error: 
			// " error C2079: 'traits' uses undefined struct 'zt::core::FunctionTraits<_T1>' " (MSVC)
			[[maybe_unused]] Traits traits;
		};

		testFunc(ExampleFunction);
	}
}