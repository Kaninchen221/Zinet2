#pragma once

#include "Zinet/RHI/OpenGL/ZtRendererContext.hpp"
#include "Zinet/RHI/OpenGL/ZtOpenGLResolver.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::opengl::tests
{

	class RendererContextTests : public ::testing::Test
	{
	protected:

		RendererContextTests()
		{
		}

		~RendererContextTests() override
		{
		}

		void SetUp() override
		{
			window.create();
			OpenGLResolver::Init();
		}

		void TearDown() override
		{
		}

		wd::GLFW glfw;
		wd::Window window;
		RendererContext rendererContext;
	};

	TEST_F(RendererContextTests, GetViewportTest)
	{
		[[maybe_unused]] const Viewport& constViewport = rendererContext.getViewport();
		[[maybe_unused]] Viewport& viewport = rendererContext.getViewport();
	}

	TEST_F(RendererContextTests, ClearColorTest)
	{
		const Vector4f ExpectedColor = { 0.21f, 1.f, 1.f, 0.534f };
		rendererContext.setClearColor(ExpectedColor);
		const Vector4f& ActualColor = rendererContext.getClearColor();
		rendererContext.applyClearColor();

		EXPECT_EQ(ExpectedColor, ActualColor);
	}

}