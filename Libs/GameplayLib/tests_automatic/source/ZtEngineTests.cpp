#pragma once

#include "Zinet/Gameplay/ZtEngine.hpp"

#include "Zinet/Core/ZtClock.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class EngineTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Engine engine;
	};

	TEST_F(EngineTests, PassTest)
	{
		ASSERT_TRUE(engine.init());

		core::Clock turnOffTest;
		turnOffTest.start();

		while (engine.shouldLoop())
		{
			if (turnOffTest.getElapsedTime().getAsSeconds() > 0.1f)
				engine.turnOff();

			engine.loop();
		}

		engine.deinit();
	}
}