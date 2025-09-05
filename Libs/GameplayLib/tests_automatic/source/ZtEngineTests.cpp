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

		std::thread thread
		{
			[&engine = engine]()
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1ms);
				engine.stopLooping();
			}
		};

		engine.loop();

		thread.join();

		engine.deinit();
	}
}