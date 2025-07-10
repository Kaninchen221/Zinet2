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
		}

		void TearDown() override
		{
		}

		Engine engine;
	};

	TEST_F(EngineTests, PassTest)
	{

	}
}