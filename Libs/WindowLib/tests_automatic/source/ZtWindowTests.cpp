#pragma once

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtTypeTraits.hpp"

#include <gtest/gtest.h>

namespace zt::wd::tests
{

	class WindowTests : public ::testing::Test
	{
	protected:

		GLFW glfw;
		Window window{};

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(WindowTests, Create)
	{
		ASSERT_TRUE(window.create());

		GLFWwindow* internalWindow = window.getInternal();
		ASSERT_TRUE(internalWindow);
	}

	TEST_F(WindowTests, BindFramebufferSizeCallbackTest)
	{
		ASSERT_TRUE(window.create());

		GLFWwindow* glfwWindow = window.getInternal();
		GLFWframebuffersizefun actualPointer = glfwSetFramebufferSizeCallback(glfwWindow, nullptr);
		GLFWframebuffersizefun expectedPointer = &Window::FramebufferSizeCallback;

		ASSERT_EQ(actualPointer, expectedPointer);
	}

	TEST_F(WindowTests, IsOpenTest)
	{
		ASSERT_TRUE(window.create());
	
		bool isOpen = window.isOpen();
		ASSERT_TRUE(isOpen);
	
		window.requestCloseWindow();
	
		isOpen = window.isOpen();
		ASSERT_FALSE(isOpen);
	}
	
	TEST_F(WindowTests, ShouldBeClosedTest)
	{
		ASSERT_TRUE(window.create());
		bool shouldBeClosed = window.shouldBeClosed();
	
		ASSERT_FALSE(shouldBeClosed);
	}
	
	TEST_F(WindowTests, GetEventTest)
	{
		auto testGetters = core::TestGetters<WindowEvents, Window>(&Window::getWindowEvents, &Window::getWindowEvents, window);
		EXPECT_TRUE(testGetters);

		[[maybe_unused]] WindowEvents& windowEvents = window.getWindowEvents();
	}

	TEST_F(WindowTests, GetWindowResizedCallback)
	{
		auto callback = window.getWindowResizedCallback();
		EXPECT_EQ(callback, nullptr);
	}

	TEST_F(WindowTests, GetWindowResizedCallbackUserPointer)
	{
		void* userPointer = window.getWindowResizedCallbackUserPointer();
		EXPECT_EQ(userPointer, nullptr);
	}

	TEST_F(WindowTests, SetWindowResizedCallback)
	{
		struct S { static void Callback(void* userPointer, const Vector2i&) { if (!userPointer) FAIL() << "userPointer must be not nullptr"; } };
		S s;
		window.setWindowResizedCallback(&s, &S::Callback);

		auto callback = window.getWindowResizedCallback();
		EXPECT_TRUE(callback);

		void* userPointer = window.getWindowResizedCallbackUserPointer();
		EXPECT_EQ(userPointer, &s);
	}

	TEST_F(WindowTests, GetWindowSize)
	{
		typedef Vector2i(Window::* ExpectedFunctionDeclaration)() const;
		using FunctionDeclaration = decltype(&Window::getSize);

		static_assert(std::is_same_v<ExpectedFunctionDeclaration, FunctionDeclaration>);
	}

	TEST_F(WindowTests, IsMinimized)
	{
		typedef bool(Window::* ExpectedFunctionDeclaration)() const;
		using FunctionDeclaration = decltype(&Window::isMinimized);

		static_assert(std::is_same_v<ExpectedFunctionDeclaration, FunctionDeclaration>);
	}
}