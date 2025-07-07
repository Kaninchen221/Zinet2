#pragma once

#include "Zinet/Gameplay/ZtGameplayLoop.hpp"

#include "Zinet/Core/ZtClock.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class GameplayLoopTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		GameplayLoop gameplayLoop;
	};

	TEST_F(GameplayLoopTests, PassTest)
	{
		ASSERT_TRUE(gameplayLoop.init());

		core::Clock turnOffTest;
		turnOffTest.start();

		while (gameplayLoop.shouldLoop())
		{
			if (turnOffTest.getElapsedTime().getAsSeconds() > 0.1f)
				gameplayLoop.turnOff();

			gameplayLoop.loop();
		}

		gameplayLoop.shutdown();
	}
}