#pragma once

#include "Zinet/SoftwareRenderer/ZtTexel.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	class TexelTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(TexelTests, CompareTest)
	{
		const Texel texel1{ 255, 0, 0, 255 };
		const Texel texel2{ 255, 0, 0, 255 };

		EXPECT_TRUE(texel1 == texel2);

		const Texel texel3{ 0, 255, 0, 255 };

		EXPECT_FALSE(texel1 == texel3);
	}
}
