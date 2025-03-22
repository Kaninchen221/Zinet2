#pragma once

#include "Zinet/GameplayLib/ZtPlayerInput.hpp"

#include <gtest/gtest.h>

#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay_lib::tests
{

	class PlayerInputTests : public ::testing::Test
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("PlayerInputTests");

		PlayerInputTests()
		{
		}

		~PlayerInputTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		PlayerInput playerInput;
	};

	TEST_F(PlayerInputTests, Test)
	{
	}
}