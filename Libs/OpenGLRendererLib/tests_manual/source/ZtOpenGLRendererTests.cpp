#pragma once

#include "Zinet/OpenGLRenderer/ZtOpenGLRenderer.hpp"

#include <gtest/gtest.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

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
			renderTarget.createEmpty({ 1920, 1080 }, sf::ColorFormat::R8G8B8A8_SRGB);
			renderTarget.fill(WhiteColor);

			softwareRenderer.draw(drawInputInfo, renderTarget);
		}

		void TearDown() override
		{
		}
		
		sf::DrawInputInfo drawInputInfo
		{
			// (0,0) point is in upper left corner
			.drawMode = sf::DrawMode::Triangles,
			.antialiasing = false,
			.vertices = std::vector<sf::Vertex>{
				sf::Vertex{ { .30f, .25f }, RedColor },
				sf::Vertex{ { .80f, .25f }, GreenColor },
				sf::Vertex{ { .25f, .75f }, BlueColor },
				sf::Vertex{ { .75f, .75f }, BlackColor }
			},
			.indices = {
				0, 1, 2,
				1, 3, 2
			},
			.vertexShader = sf::VertexShader{}
		};

		OpenGLRenderer openGLRenderer;
		sf::RenderTarget renderTarget;
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

		openGLRenderer.preRender();

		openGLRenderer.setupTexture(renderTarget.getResolution(), renderTarget.get());
		while (!window.shouldBeClosed())
		{
			event.pollEvents();

			openGLRenderer.render();

			window.swapBuffers();
		}
		openGLRenderer.postRender();
	}
}