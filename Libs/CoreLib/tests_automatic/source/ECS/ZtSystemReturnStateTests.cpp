#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtSystemReturnState.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSSystemReturnStateTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(ECSSystemReturnStateTests, Test)
	{
		using Level = SystemReturnState::Level;

		const Level expectedLevel = Level::Warn;
		const std::string expectedMessage = "WarningMessage";

		const SystemReturnState returnState{ expectedLevel, expectedMessage };
		EXPECT_EQ(expectedLevel, returnState.getLevel());
		EXPECT_EQ(expectedMessage, returnState.getMessage());
	}
}