#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtFunctionTraits.hpp"

namespace zt::core::tests
{
	class FunctionTraitsTests : public ::testing::Test
	{
	protected:

		static int32_t ExampleFunction(float, int64_t) { return {}; }
	};

	TEST_F(FunctionTraitsTests, Test)
	{
		using Traits = FunctionTraits<decltype(ExampleFunction)>;

		static_assert(std::is_same_v<Traits::ReturnT, int32_t>);
		static_assert(std::is_same_v<Traits::ArgsTs<0>, float>);
		static_assert(std::is_same_v<Traits::ArgsTs<1>, int64_t>);

		[[maybe_unused]]
		Traits::ReturnT value = ExampleFunction(Traits::ArgsTs<0>{}, Traits::ArgsTs<1>{});
	}
}