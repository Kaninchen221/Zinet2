#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/OpenGL/ZtViewport.hpp"
#include "Zinet/RHI/OpenGL/ZtOpenGLResolver.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::opengl::tests
{

	class ViewportTests : public ::testing::Test
	{
	protected:

		ViewportTests()
		{
		}

		~ViewportTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Viewport viewport;
	};

	TEST_F(ViewportTests, SetGetSizeTest)
	{
		const ZtInt ExpectedWidth = 100;
		const ZtInt ExpectedHeight = 120;
		viewport.setSize({ ExpectedWidth, ExpectedHeight });
		const auto& [ActualWidth, ActualHeigh] = viewport.getSize();

		EXPECT_EQ(ActualWidth, ExpectedWidth);
		EXPECT_EQ(ActualHeigh, ExpectedHeight);
	}

	TEST_F(ViewportTests, SetGetPositionTest)
	{
		const ZtInt ExpectedPositionX = 4;
		const ZtInt ExpectedPositionY = 8;
		viewport.setPosition({ ExpectedPositionX, ExpectedPositionY });
		const auto& [ActualPositionX, ActualPositionY] = viewport.getPosition();

		EXPECT_EQ(ExpectedPositionX, ActualPositionX);
		EXPECT_EQ(ExpectedPositionY, ActualPositionY);
	}

	TEST_F(ViewportTests, ApplyTest)
	{
		wd::GLFW glfw;

		wd::Window window;
		window.create();

		OpenGLResolver::Init();

		viewport.setSize({ 100, 100 });
		viewport.setPosition({ 0, 0 });
		viewport.apply();
	}
}

#endif // ZINET_USE_OPENGL