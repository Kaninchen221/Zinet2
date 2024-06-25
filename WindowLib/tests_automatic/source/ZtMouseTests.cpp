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
			GLFW::Init();
		}

		void TearDown() override
		{
			GLFW::Deinit();
		}
	};

	TEST_F(MouseTests, GetWindow)
	{
		const Window* actualWindow = mouse.getWindow();
		ASSERT_EQ(actualWindow, &window);
	}

	TEST_F(MouseTests, GetButtonsEvents)
	{
		[[maybe_unused]] const std::vector<MouseButtonEvent>& buttonsEvents = mouse.getButtonsEvents();
	}

	TEST_F(MouseTests, SetMaxRememberedButtonsEvents)
	{
		size_t expectedMaxRememberedButtonsEvents = 4u;
		mouse.setMaxRememberedButtonsEvents(expectedMaxRememberedButtonsEvents);
		size_t actualMaxRememberedButtonsEvents = mouse.getMaxRememberedButtonsEvents();

		ASSERT_EQ(expectedMaxRememberedButtonsEvents, actualMaxRememberedButtonsEvents);

		actualMaxRememberedButtonsEvents = mouse.getButtonsEvents().size();

		ASSERT_EQ(expectedMaxRememberedButtonsEvents, actualMaxRememberedButtonsEvents);
	}

	TEST_F(MouseTests, ButtonCallback)
	{
		window.create();
		MouseButtonEvent expectedButtonEvent{};
		Mouse::ButtonCallback(window.getInternal(), static_cast<int>(expectedButtonEvent.button), static_cast<int>(expectedButtonEvent.type), 0);
		const std::vector<MouseButtonEvent>& buttonsEvents = mouse.getButtonsEvents();
		size_t actualButtonsEventsCount = buttonsEvents.size();
		size_t expectedButtonsEventsCount = 1u;

		ASSERT_EQ(actualButtonsEventsCount, expectedButtonsEventsCount);

		MouseButtonEvent actualButtonEvent = buttonsEvents[0];
		ASSERT_EQ(expectedButtonEvent, actualButtonEvent);
	}

	TEST(Mouse, PositionCallback)
	{
		GLFW::Init();

		Window window;
		window.create();
		Vector2d expectedPosition{ 34.0, 2.0 };
		Mouse::PositionCallback(window.getInternal(), expectedPosition.x, expectedPosition.y);

		Event& event = window.getEvent();
		Mouse& mouse = event.getMouse();
		const std::vector<MousePositionEvent>& positions = mouse.getPositionEvents();
		MousePositionEvent positionEvent = positions[0];
		Vector2d actualPosition = positionEvent.position;
		ASSERT_EQ(expectedPosition, actualPosition);

		mouse.setMaxRememberedPositionEvents(2u);
		Vector2d expectedSecondPosition{ 67.0, 27.0 };
		Mouse::PositionCallback(window.getInternal(), expectedSecondPosition.x, expectedSecondPosition.y);
		Vector2d actualSecondPosition = positions[0].position;
		ASSERT_EQ(expectedSecondPosition, actualSecondPosition);

		GLFW::Deinit();
	}

	TEST_F(MouseTests, GetPositionEvents)
	{
		[[maybe_unused]] const std::vector<MousePositionEvent>& positions = mouse.getPositionEvents();
	}

	TEST_F(MouseTests, SetMaxRememberedPositionEvents)
	{
		size_t expectedMaxRememberedPositionEvents = 3u;
		mouse.setMaxRememberedPositionEvents(expectedMaxRememberedPositionEvents);
		size_t actualMaxRememberedPositionEvents = mouse.getMaxRememberedPositionEvents();

		ASSERT_EQ(actualMaxRememberedPositionEvents, expectedMaxRememberedPositionEvents);
	}

}