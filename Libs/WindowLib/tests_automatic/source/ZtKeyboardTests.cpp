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

	TEST_F(KeyboardTests, PushEventAndClearTest)
	{
		auto expectedKey = KeyboardKey::F14;
		int expectedScanCode = 34232;
		auto expectedType = KeyboardEventType::Released;
		auto expectedMods = KeyboardMods::NumLock;
		keyboard.pushEvent(expectedKey, expectedScanCode, expectedType, expectedMods);

		auto& events = keyboard.getEvents();

		auto& lastEvent = events.front();
		EXPECT_EQ(lastEvent.key, expectedKey);
		EXPECT_EQ(lastEvent.type, expectedType);
		EXPECT_EQ(lastEvent.mods, expectedMods);

		keyboard.clearEvents();
		EXPECT_TRUE(events.empty());

		keyboard.pushEvent(expectedKey, expectedScanCode, expectedType, expectedMods);
		keyboard.pushEvent(expectedKey, expectedScanCode, expectedType, expectedMods);
		EXPECT_EQ(events.size(), 2);
	}
}