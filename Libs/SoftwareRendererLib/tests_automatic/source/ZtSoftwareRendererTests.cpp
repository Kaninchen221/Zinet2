#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::software_renderer::tests
{

	class SoftwareRendererTests : public ::testing::Test
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("SoftwareRendererTests");

		SoftwareRendererTests()
		{
		}

		~SoftwareRendererTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		SoftwareRenderer softwareRenderer;

		DrawInfo getInputDrawInfoRect() const
		{
			return DrawInfo
			{
				// (0,0) point is in upper left corner
				.drawMode = DrawMode::Points,
				.vertices = std::vector<Vertex>{
					Vertex{ { .30f, .25f }, RedColor },
					Vertex{ { .80f, .25f }, GreenColor },
					Vertex{ { .25f, .75f }, BlueColor },
					Vertex{ { .75f, .75f }, BlackColor }
				},
				.indices = {
					0, 1, 2,
					1, 3, 2
				}
			};
		}

		DrawInfo getInputDrawInfoTriangle() const
		{
			return DrawInfo
			{
				// (0,0) point is in upper left corner
				.drawMode = DrawMode::Triangles,
				.vertices = std::vector<Vertex>{
					Vertex{ { -0.75f, .25f }, RedColor },
					Vertex{ { .75f, .25f }, GreenColor },
					Vertex{ { .25f, 1.75f }, BlueColor }
				},
				.indices = {
					0, 1, 2
				}
			};
		}

		DrawInfo getInputDrawInfoLines() const
		{
			return DrawInfo
			{
				// (0,0) point is in upper left corner
				.drawMode = DrawMode::Lines,
				.vertices = std::vector<Vertex>{
					Vertex{ { .30f, .25f }, RedColor },
					Vertex{ { .80f, .25f }, GreenColor },
					Vertex{ { .25f, .75f }, BlueColor },
					Vertex{ { .75f, .75f }, BlackColor }
				},
				.indices = {
					0, 2, 
					1, 3
				}
			};
		}

		void createRenderTarget(RenderTarget& renderTarget, Color fillColor = WhiteColor, Vector2i size = Vector2i{ 512, 512 })
		{
			const ColorFormat colorFormat = ColorFormat::R8G8B8A8_SRGB;
			bool renderTargetCreateResult = renderTarget.createEmpty(size, colorFormat);
			ASSERT_TRUE(renderTargetCreateResult);
			renderTarget.fill(fillColor);
		}
	};

	TEST_F(SoftwareRendererTests, InfoTest)
	{
		constexpr const auto drawInfoSize = sizeof(DrawInfo);
		Logger->info("DrawInfo size: {}", drawInfoSize);
	}

	TEST_F(SoftwareRendererTests, DrawPointsTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget, WhiteColor, { 20, 20 });

		auto drawInfo = getInputDrawInfoRect();
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_points_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawTrianglesLinesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		auto drawInfo = getInputDrawInfoRect();
		drawInfo.drawMode = DrawMode::TrianglesLines;
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_triangles_lines_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawTrianglesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget, WhiteColor, Vector2i{ 1920, 1080 });

		auto drawInfo = getInputDrawInfoRect();
		drawInfo.drawMode = DrawMode::Triangles;
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_triangles_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawTriangleWithCustomShadersTest)
	{
		VertexShader::ProcessVertexCallableT vertexShaderProcess =
		[](const VertexShader&, Vertex& vertex)
		{
			vertex.position.x += 0.5f;
		};

		FragmentShader::ProcessFragmentCallableT fragmentShaderProcess =
		[](const FragmentShader&, Pixel& fragment)
		{
			fragment.color = BlackColor;
		};

		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		auto drawInfo = getInputDrawInfoTriangle();
		drawInfo.shaderProgram.vertexShader.processVertex = vertexShaderProcess;
		drawInfo.shaderProgram.fragmentShader.processFragment = fragmentShaderProcess;
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_triangle_with_custom_shaders_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawLinesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		auto drawInfo = getInputDrawInfoLines();
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_lines_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawTexturedRectTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget, WhiteColor, { 1024, 1024 });

		FragmentShader::ProcessFragmentCallableT fragmentShaderProcess =
			[](const FragmentShader& fragmentShader, Pixel& fragment)
		{
			fragment.color = sampleTexture(fragmentShader.textures[0], fragment.uv);
		};

		RenderTarget sourceRenderTarget;
		const std::filesystem::path savePath = core::Paths::CurrentProjectRootPath() / "test_files" / "uv_texture.png";
		const auto loadResult = sourceRenderTarget.loadFromFilePNG(savePath);
		ASSERT_TRUE(loadResult);

		auto drawInfo = getInputDrawInfoRect();

		drawInfo.vertices[0].position = Vector2f{ .1f, .05f };
		drawInfo.vertices[1].position = Vector2f{ .95f, .05f };
		drawInfo.vertices[2].position = Vector2f{ .05f, .95f };
		drawInfo.vertices[3].position = Vector2f{ .90f, .95f };

		drawInfo.vertices[0].uv = Vector2f{ 0.50f, 0.25f };
		drawInfo.vertices[1].uv = Vector2f{ 0.25f, 0.25f };
		drawInfo.vertices[2].uv = Vector2f{ 0.50f, 0.50f };
		drawInfo.vertices[3].uv = Vector2f{ 0.25f, 0.50f };

		drawInfo.drawMode = DrawMode::Triangles;
		drawInfo.shaderProgram.fragmentShader.textures.push_back(sourceRenderTarget);
		drawInfo.shaderProgram.fragmentShader.processFragment = fragmentShaderProcess;
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_texture_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawRenderTargetInRenderTargetTest)
	{
		RenderTarget destRenderTarget;
		createRenderTarget(destRenderTarget, WhiteColor, { 64, 64 });

		RenderTarget srcRenderTarget;
		createRenderTarget(srcRenderTarget, RedColor, { 64, 64 });

		{
			Logger->info("Draw first rect");
			DrawRenderTargetInfo drawInfo {
				.srcRenderTarget = srcRenderTarget,
				.position = { 32, 32 }
			};

			softwareRenderer.drawRenderTarget(drawInfo, destRenderTarget);
			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_render_target_result_00.png";
			const bool saveResult = destRenderTarget.saveToFilePNG(path);
			ASSERT_TRUE(saveResult);
		}

		{
			Logger->info("Draw second rect");
			DrawRenderTargetInfo drawInfo {
				.srcRenderTarget = srcRenderTarget,
				.position = { -32, -32 }
			};
			srcRenderTarget.fill(BlueColor);
			softwareRenderer.drawRenderTarget(drawInfo, destRenderTarget);


			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_render_target_result_01.png";
			const bool saveResult = destRenderTarget.saveToFilePNG(path);
			ASSERT_TRUE(saveResult);
		}

		{
			Logger->info("Draw third rect");
			RenderTarget greenRect;
			createRenderTarget(greenRect, GreenColor, { 32, 32 });

			DrawRenderTargetInfo drawInfo{
				.srcRenderTarget = greenRect,
				.position = { 16, 16 }
			};
			softwareRenderer.drawRenderTarget(drawInfo, destRenderTarget);

			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_render_target_result_02.png";
			const bool saveResult = destRenderTarget.saveToFilePNG(path);
			ASSERT_TRUE(saveResult);
		}

		{
			Logger->info("Draw fourth rect");
			RenderTarget blackRect;
			createRenderTarget(blackRect, BlackColor, { 16, 16 });

			DrawRenderTargetInfo drawInfo{
				.srcRenderTarget = blackRect,
				.position = { 24, 24 }
			};
			softwareRenderer.drawRenderTarget(drawInfo, destRenderTarget);

			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_render_target_result_03.png";
			const bool saveResult = destRenderTarget.saveToFilePNG(path);
			ASSERT_TRUE(saveResult);
		}
	}
}