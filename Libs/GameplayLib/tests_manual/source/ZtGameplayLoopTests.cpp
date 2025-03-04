#pragma once

#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class GameplayLoopTests : public ::testing::Test
	{
	protected:

		GameplayLoopTests()
		{
		}

		~GameplayLoopTests() override {
		}

		void SetUp() override {
		}

		void TearDown() override {
		}

	};

	TEST_F(GameplayLoopTests, PassTest)
	{

	}
}