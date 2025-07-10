#pragma once

#include "Zinet/Gameplay/ZtEngine.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class EngineTests : public ::testing::Test
	{
	protected:

		EngineTests()
		{
		}

		~EngineTests() override
		{
		}

		void SetUp() override
		{
			ASSERT_TRUE(engine.init());
		}

		void TearDown() override
		{
			engine.deinit();
		}

		Engine engine;
	};

	TEST_F(EngineTests, PassTest)
	{
		while (engine.shouldLoop())
		{
			engine.loop();
		}
	}
}