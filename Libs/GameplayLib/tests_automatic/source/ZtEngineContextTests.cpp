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

	TEST_F(EngineContextTests, InitTest)
	{
		ASSERT_TRUE(engineContext.init());
		auto& instance = EngineContext::Get();
		ASSERT_TRUE(&instance);
		engineContext.deinit();
	}

	TEST_F(EngineContextTests, SystemTest)
	{
		ASSERT_TRUE(engineContext.init());
		engineContext.addSystem<System>();
		System* system = engineContext.getSystem<System>();
		ASSERT_TRUE(system);
		engineContext.deinit();
	}
}