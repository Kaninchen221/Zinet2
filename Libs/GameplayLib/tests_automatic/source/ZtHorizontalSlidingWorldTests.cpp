#pragma once

#include "Zinet/GameplayLib/ZtHorizontalSlidingWorld.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class HorizontalSlidingWorldTests : public ::testing::Test
	{
	protected:

		HorizontalSlidingWorldTests()
		{
		}

		~HorizontalSlidingWorldTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		HorizontalSlidingWorld horizontalSlidingWorld;
	};

	TEST_F(HorizontalSlidingWorldTests, GetDrawableNodes)
	{
		[[maybe_unused]] const std::vector<std::shared_ptr<TileMap>>& drawableNodes = horizontalSlidingWorld.getGroundSlicesNodes();
	}
}