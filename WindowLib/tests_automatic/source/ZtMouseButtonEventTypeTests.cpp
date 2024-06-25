#pragma once

#include "Zinet/Window/ZtMouseButtonEventType.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class MouseButtonEventTypeTests : public ::testing::Test
	{
	protected:

		MouseButtonEventType mouseButtonEventType{};

	};

	TEST_F(MouseButtonEventTypeTests, Assign)
	{
		mouseButtonEventType = MouseButtonEventType::Pressed;
	}

}