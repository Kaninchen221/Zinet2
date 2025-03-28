#pragma once

#include "Zinet/Window/ZtWindowEvents.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtTypeTraits.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class EventTests : public ::testing::Test
	{
	protected:

		Window window;
		WindowEvents windowEvents{ window };

		static_assert(!std::is_default_constructible_v<WindowEvents>);

		void SetUp() override
		{
			GLFW::Init();
		}

		void TearDown() override
		{
			GLFW::Deinit();
		}
	};

	TEST_F(EventTests, GetWindow)
	{
		const Window* actualWindow = windowEvents.getWindow();
		EXPECT_EQ(actualWindow, &window);
	}

	TEST_F(EventTests, PollEvents)
	{
		auto& keyboard = windowEvents.getKeyboard();
		auto& mouse = windowEvents.getMouse();

		keyboard.pushEvent(KeyboardKey{}, 0, KeyboardEventType{}, KeyboardMods{});
		EXPECT_EQ(keyboard.getEvents().size(), 1);

		mouse.pushButtonEvent(0, 0, 0);
		EXPECT_EQ(mouse.getButtonsEvents().size(), 1);

		mouse.pushPositionEvent(0, 0);
		EXPECT_EQ(mouse.getPositionEvents().size(), 1);

		windowEvents.pollEvents();

		EXPECT_EQ(keyboard.getEvents().size(), 0);
		EXPECT_EQ(mouse.getButtonsEvents().size(), 0);
	}

	TEST_F(EventTests, GetKeyboard)
	{
		auto testGetters = core::TestGetters<Keyboard, WindowEvents>(&WindowEvents::getKeyboard, &WindowEvents::getKeyboard, windowEvents);
		EXPECT_TRUE(testGetters);

		[[maybe_unused]] Keyboard& keyboard = windowEvents.getKeyboard();
	}

	TEST_F(EventTests, GetMouse)
	{
		auto testGetters = core::TestGetters<Mouse, WindowEvents>(&WindowEvents::getMouse, &WindowEvents::getMouse, windowEvents);
		EXPECT_TRUE(testGetters);

		[[maybe_unused]] Mouse& mouse = windowEvents.getMouse();
	}

}