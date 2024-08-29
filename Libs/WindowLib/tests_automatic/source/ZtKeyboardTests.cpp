#pragma once

#include "Zinet/Window/ZtKeyboard.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class KeyboardTests : public ::testing::Test
	{
	protected:

		GLFW glfw;
		Window window;
		Keyboard keyboard{ window };

		static_assert(!std::is_default_constructible_v<Keyboard>);

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(KeyboardTests, GetWindow)
	{
		const Window* windowPointer = keyboard.getWindow();
		EXPECT_EQ(windowPointer, &window);
	}

	TEST_F(KeyboardTests, IsPressed)
	{
		window.create();
		bool isPressed = keyboard.isPressed(KeyboardKey::F4);
		ASSERT_FALSE(isPressed);
	}

	TEST_F(KeyboardTests, IsReleased)
	{
		window.create();
		bool isReleased = keyboard.isReleased(KeyboardKey::F2);
		ASSERT_TRUE(isReleased);
	}

	TEST_F(KeyboardTests, SetMaximumRememberedEvents)
	{
		size_t expectedMaximumRememberedEvents = 9u;
		keyboard.setMaximumRememberedEvents(expectedMaximumRememberedEvents);
		size_t actualMaximumRememberedEvents = keyboard.getMaximumRememberedEvents();

		ASSERT_EQ(expectedMaximumRememberedEvents, actualMaximumRememberedEvents);

		const std::vector<KeyboardEvent>& events = keyboard.getEvents();
		size_t actualEventsCount = events.size();
		size_t expectedEventsCount = expectedMaximumRememberedEvents;

		ASSERT_EQ(actualEventsCount, expectedEventsCount);
	}

}