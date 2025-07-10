#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class EngineContextTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		EngineContext engineContext;
	};

	TEST_F(EngineContextTests, PassTest)
	{
		ASSERT_TRUE(engineContext.init());
		engineContext.deinit();
	}
}