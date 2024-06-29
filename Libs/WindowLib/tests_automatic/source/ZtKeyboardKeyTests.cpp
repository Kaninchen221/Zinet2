#pragma once

#include "Zinet/Window/ZtKeyboardKey.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class KeyboardKeyTests : public ::testing::Test
	{
	protected:

		KeyboardKey keyboardKey{};

	};

	TEST_F(KeyboardKeyTests, Construct)
	{}

}