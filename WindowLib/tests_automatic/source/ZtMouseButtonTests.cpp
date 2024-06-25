#pragma once

#include "Zinet/Window/ZtMouseButton.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class MouseButtonTests : public ::testing::Test
	{
	protected:

		MouseButton button{};

	};

	TEST_F(MouseButtonTests, Assign)
	{
		button = MouseButton::RIGHT;
	}

}