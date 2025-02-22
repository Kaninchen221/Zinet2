#pragma once

#include "Zinet/OpenGLRenderer/ZtOpenGLRenderer.hpp"

#include <gtest/gtest.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

namespace zt::opengl_renderer::tests
{
	namespace sf = software_renderer;

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
		}

		void TearDown() override
		{
		}

		OpenGLRenderer openGLRenderer;
		sf::SoftwareRenderer softwareRenderer;
	};

	TEST_F(OpenGLRendererTests, Test)
	{
		wd::GLFW glfw;
		wd::GLFW::UnhideWindow();

		wd::Window window;
		wd::Event event{ window };

		window.create();

		if (!openGLRenderer.init())
			FAIL() << "OpenGL renderer failed init";

		while (!window.shouldBeClosed())
		{
			event.pollEvents();

			openGLRenderer.render();

			window.swapBuffers();
		}
	}
}