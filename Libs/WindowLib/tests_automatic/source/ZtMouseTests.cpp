#pragma once

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtMouse.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"
#include "Zinet/Window/ZtMousePositionEvent.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include <gtest/gtest.h>

#include <glm/vec2.hpp>
#include <glm/vector_relational.hpp>

namespace zt::wd::tests
{

	class MouseTests : public ::testing::Test
	{
	protected:

		Window window;
		Mouse mouse{ window };

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(MouseTests, GetWindowTest)
	{
		const Window* actualWindow = mouse.getWindow();
		ASSERT_EQ(actualWindow, &window);
	}

	TEST_F(MouseTests, PushButtonEventTest)
	{
		MouseButtonEvent expectedButtonEvent{};
		mouse.pushButtonEvent(static_cast<int>(expectedButtonEvent.button), static_cast<int>(expectedButtonEvent.type), 0);
		const std::vector<MouseButtonEvent>& buttonsEvents = mouse.getButtonsEvents();
		size_t actualButtonsEventsCount = buttonsEvents.size();
		ASSERT_EQ(actualButtonsEventsCount, 1);

		const MouseButtonEvent& actualButtonEvent = buttonsEvents.front();
		ASSERT_EQ(expectedButtonEvent, actualButtonEvent);
	}

	TEST_F(MouseTests, PushPositionEventTest)
	{
		Vector2d expectedPosition{ 34.0, 2.0 };
		mouse.pushPositionEvent(expectedPosition.x, expectedPosition.y);

		const std::vector<MousePositionEvent>& positions = mouse.getPositionEvents();
		Vector2d actualPosition = positions.front().position;
		ASSERT_EQ(expectedPosition, actualPosition);

		Vector2d expectedSecondPosition{ 67.0, 27.0 };
		mouse.pushPositionEvent(expectedSecondPosition.x, expectedSecondPosition.y);
		Vector2d actualSecondPosition = positions.front().position;
		ASSERT_EQ(expectedSecondPosition, actualSecondPosition);
	}
}