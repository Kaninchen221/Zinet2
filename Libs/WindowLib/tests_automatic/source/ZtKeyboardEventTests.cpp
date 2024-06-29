#pragma once

#include "Zinet/Window/ZtKeyboardEvent.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class KeyboardEventTests : public ::testing::Test
	{
	protected:

		KeyboardEvent keyboardEvent{};

	};

	TEST_F(KeyboardEventTests, Type)
	{
		[[maybe_unused]] KeyboardEventType type = keyboardEvent.type;
	}

	TEST_F(KeyboardEventTests, Key)
	{
		[[maybe_unused]] KeyboardKey key = keyboardEvent.key;
	}

}