#pragma once

#include "Zinet/OpenGLRenderer/ZtOpenGLRenderer.hpp"

#include <gtest/gtest.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtPaths.hpp"

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
		
		void changeVertices(sf::DrawInfo& drawInfo);

		sf::DrawInfo getDrawInfoRect() const {
			return sf::DrawInfo
			{
				// (0,0) point is in upper left corner
				.drawMode = sf::DrawMode::Triangles,
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
		}

		OpenGLRenderer openGLRenderer;
		sf::RenderTarget renderTarget;
		sf::SoftwareRenderer softwareRenderer;
	};

	void OpenGLRendererTests::changeVertices(sf::DrawInfo& drawInfo)
	{

		static float direction = 1.f;
		auto& vertex = drawInfo.vertices[0];

		const float min = -0.1f;
		const float max = 0.30f;
		if (vertex.position.x <= min || vertex.position.x >= max)
		{
			direction *= -1.f;
			vertex.position.x = std::clamp(vertex.position.x, min, max);
		}

		vertex.position.x += direction * 0.005f;
	}

	TEST_F(OpenGLRendererTests, TexturedTest)
	{
		wd::GLFW glfw;
		wd::GLFW::UnhideWindow();

		wd::Window window;
		wd::Event event{ window };

		window.create();

		if (!openGLRenderer.init(window))
			FAIL() << "OpenGL renderer failed init";

		// Ground
		sf::RenderTarget groundTexture;
		const std::filesystem::path groundTexturePath = core::Paths::CurrentProjectRootPath() / "test_files" / "ground.png";
		groundTexture.loadFromFilePNG(groundTexturePath);

		const float min = .42f;
		const float max = .68f;
		sf::DrawInfo groundDrawInfo
		{
			.drawMode = sf::DrawMode::Triangles,
			.vertices = std::vector<sf::Vertex>{
				sf::Vertex{ { min, min }, sf::RedColor, Vector2f{ 0.f, 0.f } },
				sf::Vertex{ { max, min }, sf::GreenColor, Vector2f{ 1.f, 0.f } },
				sf::Vertex{ { min, max }, sf::BlueColor, Vector2f{ 0.f, 1.f } },
				sf::Vertex{ { max, max }, sf::BlackColor, Vector2f{ 1.f, 1.f } }
			},
			.indices = {
				0, 1, 2,
				1, 3, 2
			}
		};
		groundDrawInfo.shaderProgram.fragmentShader.processFragment = sf::FragmentShaderSampleTextureProcess;
		groundDrawInfo.shaderProgram.fragmentShader.textures.push_back(groundTexture);

		// Character
		sf::RenderTarget characterTexture;
		const std::filesystem::path characterTexturePath = core::Paths::CurrentProjectRootPath() / "test_files" / "player.png";
		characterTexture.loadFromFilePNG(characterTexturePath);

		auto characterDrawInfo = groundDrawInfo;
		characterDrawInfo.vertices[0].position = Vector2f{ .52f, .50f };
		characterDrawInfo.vertices[1].position = Vector2f{ .60f, .50f };
		characterDrawInfo.vertices[2].position = Vector2f{ .52f, .60f };
		characterDrawInfo.vertices[3].position = Vector2f{ .60f, .60f };
		characterDrawInfo.shaderProgram.fragmentShader.textures[0] = characterTexture;

		// Loop
		openGLRenderer.preRender();
		while (!window.shouldBeClosed())
		{
#if ZINET_TIME_TRACE
			core::Clock clock;
			clock.start();
#endif
			event.pollEvents();

			renderTarget.fill(sf::WhiteColor);
			Logger->info("Ground 'Sprite'");
			softwareRenderer.draw(groundDrawInfo, renderTarget);
			Logger->info("Character 'Sprite'");
			softwareRenderer.draw(characterDrawInfo, renderTarget);

			openGLRenderer.setupTexture(renderTarget.getResolution(), renderTarget.get());
			openGLRenderer.render();

			window.swapBuffers();
#if ZINET_TIME_TRACE
			const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
			Logger->info("Frame took: {} milliseconds", elapsedTime);
#endif
		}
		openGLRenderer.postRender();

		const std::filesystem::path resultPath = core::Paths::CurrentProjectRootPath() / "test_files" / "textured_result.png";
		renderTarget.saveToFilePNG(resultPath);
	}
}