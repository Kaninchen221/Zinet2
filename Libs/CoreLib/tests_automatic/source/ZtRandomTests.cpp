#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtRandom.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::core::tests
{

	class RandomTests : public ::testing::Test
	{
	protected:

		Random random;
	};

	TEST_F(RandomTests, RealTest)
	{
		const int min = -30;
		const int max = 30;

		const float floatValue = random.real<float>(min, max);
		EXPECT_GE(floatValue, min);
		EXPECT_LE(floatValue, max);

		const double doubleValue = random.real<double>(min, max);
		[[maybe_unused]] 
		const double doubleValue2 = random.real<double>(min, max);
		EXPECT_GE(doubleValue, min);
		EXPECT_LE(doubleValue, max);
	}

	TEST_F(RandomTests, IntTest)
	{
		const int min = -30;
		const int max = 30;

		const int intValue = random.integer<int>(min, max);
		EXPECT_GE(intValue, min);
		EXPECT_LE(intValue, max);

		const int64_t int64Value = random.integer<int64_t>(min, max);
		EXPECT_GE(int64Value, min);
		EXPECT_LE(int64Value, max);
	}
}