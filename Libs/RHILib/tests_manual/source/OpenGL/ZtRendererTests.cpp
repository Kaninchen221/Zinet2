#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/OpenGL/ZtRenderer.hpp"
#include "Zinet/RHI/OpenGL/ZtOpenGLResolver.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtClock.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::opengl::tests
{

	class OpenGLRendererTests : public ::testing::Test
	{
	protected:

		OpenGLRendererTests()
		{
		}

		~OpenGLRendererTests() override
		{
		}

		void SetUp() override
		{
			wd::GLFW::UnhideWindow();
			window.create();
			OpenGLResolver::Init();
			rendererContext.getViewport().setSize({ 500, 500 });
			rendererContext.getViewport().setPosition({ 0, 0 });
			rendererContext.getViewport().apply();
			rendererContext.setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		}

		void TearDown() override
		{
		}

		wd::GLFW glfw;
		wd::Window window;
		Renderer renderer;
		RendererContext& rendererContext = renderer.getRendererContext();
	};

	TEST_F(OpenGLRendererTests, Test)
	{
		core::Clock clock;
		clock.start();
		while (!window.shouldBeClosed())
		{
			if (clock.getElapsedTime().getAsSeconds() > 15)
			{
				window.requestCloseWindow();
				break;
			}
			
			renderer.startRenderingFrame();
			glfwSwapBuffers(window.getInternal());
			
			// Draw here
			
			window.getEvent().pollEvents();
			renderer.endRenderingFrame();
		}
	}

}

#endif // ZINET_USE_OPENGL