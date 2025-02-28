#pragma once

#include "Zinet/OpenGLRenderer/ZtOpenGLRenderer.hpp"

#include <gtest/gtest.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include <algorithm>

namespace zt::opengl_renderer::tests
{
	namespace sf = software_renderer;

	class OpenGLRendererTests : public ::testing::Test
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("OpenGLRendererTests");

		OpenGLRendererTests()
		{
		}

		~OpenGLRendererTests() override
		{
		}

		void SetUp() override
		{
			renderTarget.createEmpty({ 1920, 1080 }, sf::ColorFormat::R8G8B8A8_SRGB);
		}

		void TearDown() override
		{
		}
		
		void changeVertices();

		sf::DrawInfo drawInputInfo
		{
			// (0,0) point is in upper left corner
			.drawMode = sf::DrawMode::Triangles,
			.antialiasing = false,
			.vertices = std::vector<sf::Vertex>{
				sf::Vertex{ { .30f, .25f }, sf::RedColor },
				sf::Vertex{ { .80f, .25f }, sf::GreenColor },
				sf::Vertex{ { .25f, .75f }, sf::BlueColor },
				sf::Vertex{ { .75f, .75f }, sf::BlackColor }
			},
			.indices = {
				0, 1, 2,
				1, 3, 2
			}
		};

		OpenGLRenderer openGLRenderer;
		sf::RenderTarget renderTarget;
		sf::SoftwareRenderer softwareRenderer;
	};

	void OpenGLRendererTests::changeVertices()
	{
		static float direction = 1.f;
		auto& vertex = drawInputInfo.vertices[0];

		const float min = -0.1f;
		const float max = 0.30f;
		if (vertex.position.x <= min || vertex.position.x >= max)
		{
			direction *= -1.f;
			vertex.position.x = std::clamp(vertex.position.x, min, max);
		}

		vertex.position.x += direction * 0.005f;
	}

	TEST_F(OpenGLRendererTests, Test)
	{
		wd::GLFW glfw;
		wd::GLFW::UnhideWindow();

		wd::Window window;
		wd::Event event{ window };

		window.create();

		if (!openGLRenderer.init(window))
			FAIL() << "OpenGL renderer failed init";

		openGLRenderer.preRender();
		while (!window.shouldBeClosed())
		{
			event.pollEvents();

			changeVertices();

			renderTarget.fill(sf::WhiteColor);
			softwareRenderer.draw(drawInputInfo, renderTarget);

			openGLRenderer.setupTexture(renderTarget.getResolution(), renderTarget.get());
			openGLRenderer.render();

			window.swapBuffers();
		}
		openGLRenderer.postRender();
	}
}