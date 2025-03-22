#pragma once

#include "Zinet/Window/ZtEvent.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtTypeTraits.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class EventTests : public ::testing::Test
	{
	protected:

		Window window;
		Event event{ window };

		static_assert(!std::is_default_constructible_v<Event>);

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
		const Window* actualWindow = event.getWindow();
		EXPECT_EQ(actualWindow, &window);
	}

	TEST_F(EventTests, PollEvents)
	{
		auto& keyboard = event.getKeyboard();
		auto& mouse = event.getMouse();

		keyboard.pushEvent(KeyboardKey{}, 0, KeyboardEventType{}, KeyboardMods{});
		EXPECT_EQ(keyboard.getEvents().size(), 1);

		mouse.pushButtonEvent(0, 0, 0);
		EXPECT_EQ(mouse.getButtonsEvents().size(), 1);

		mouse.pushPositionEvent(0, 0);
		EXPECT_EQ(mouse.getPositionEvents().size(), 1);

		event.pollEvents();

		EXPECT_EQ(keyboard.getEvents().size(), 0);
		EXPECT_EQ(mouse.getButtonsEvents().size(), 0);
	}

	TEST_F(EventTests, GetKeyboard)
	{
		auto testGetters = core::TestGetters<Keyboard, Event>(&Event::getKeyboard, &Event::getKeyboard, event);
		EXPECT_TRUE(testGetters);

		[[maybe_unused]] Keyboard& keyboard = event.getKeyboard();
	}

	TEST_F(EventTests, GetMouse)
	{
		auto testGetters = core::TestGetters<Mouse, Event>(&Event::getMouse, &Event::getMouse, event);
		EXPECT_TRUE(testGetters);

		[[maybe_unused]] Mouse& mouse = event.getMouse();
	}

}