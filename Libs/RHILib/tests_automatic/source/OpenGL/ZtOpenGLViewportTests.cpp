#pragma once

#include "Zinet/RHI/OpenGL/ZtOpenGLViewport.hpp"

#include <gtest/gtest.h>
#include "Zinet/Window/ZtWindow.hpp"

namespace zt::rhi::tests
{

	class OpenGLViewportTests : public ::testing::Test
	{
	protected:

		OpenGLViewportTests()
		{
		}

		~OpenGLViewportTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		OpenGLViewport viewport;
	};

	TEST_F(OpenGLViewportTests, SetGetSizeTest)
	{
		const ZtInt ExpectedWidth = 100;
		const ZtInt ExpectedHeight = 120;
		viewport.setSize({ ExpectedWidth, ExpectedHeight });
		const auto& [ActualWidth, ActualHeigh] = viewport.getSize();

		EXPECT_EQ(ActualWidth, ExpectedWidth);
		EXPECT_EQ(ActualHeigh, ExpectedHeight);
	}

	TEST_F(OpenGLViewportTests, SetGetPositionTest)
	{
		const ZtInt ExpectedPositionX = 4;
		const ZtInt ExpectedPositionY = 8;
		viewport.setPosition({ ExpectedPositionX, ExpectedPositionY });
		const auto& [ActualPositionX, ActualPositionY] = viewport.getPosition();

		EXPECT_EQ(ExpectedPositionX, ActualPositionX);
		EXPECT_EQ(ExpectedPositionY, ActualPositionY);
	}

	TEST_F(OpenGLViewportTests, ApplyTest)
	{
		wd::GLFW glfw;

		wd::Window window;
		window.create();

		glbinding::initialize(glfwGetProcAddress);

		viewport.setSize({ 100, 100 });
		viewport.setPosition({ 0, 0 });
		viewport.apply();
	}
}