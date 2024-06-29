#pragma once

#include "Zinet/Window/ZtMousePositionEvent.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class MousePositionEventTests : public ::testing::Test
	{
	protected:

		MousePositionEvent positionEvent{};

	};

	TEST_F(MousePositionEventTests, Position)
	{
		positionEvent.position = glm::dvec2();
	}

}