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

		DrawInputInfo drawInputInfo
		{
			// (0,0) point is in upper left corner
			.drawMode = DrawMode::Points,
			.antialiasing = false,
			.vertices = std::vector<Vertex>{ 
				Vertex{ { .25f, .25f, 0.f }, RedColor }, 
				Vertex{ { .75f, .25f, 0.f }, GreenColor }, 
				Vertex{ { .25f, .75f, 0.f }, BlueColor }, 
				Vertex{ { .75f, .75f, 0.f }, BlackColor } 
			},
			.indices = { 
				0, 1, 2, 
				1, 3, 2 
			},
			.vertexShader = VertexShader{}
		};

		void createRenderTarget(RenderTarget& renderTarget)
		{
			const Vector2ui size = { 256, 128 };
			const ColorFormat colorFormat = ColorFormat::R8G8B8A8_SRGB;
			bool renderTargetCreateResult = renderTarget.create(size, colorFormat);
			ASSERT_TRUE(renderTargetCreateResult);
			renderTarget.fill(WhiteColor);
		}
	};

	TEST_F(SoftwareRendererTests, DrawPointsTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		softwareRenderer.draw(drawInputInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_points_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawLinesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		drawInputInfo.drawMode = DrawMode::Lines;
		drawInputInfo.antialiasing = false;
		softwareRenderer.draw(drawInputInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_lines_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawLinesTestAntialiasing)
	{
		// TODO Draw with Antialiasing
	}

	TEST_F(SoftwareRendererTests, DrawTrianglesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		drawInputInfo.drawMode = DrawMode::Triangles;
		drawInputInfo.antialiasing = false;
		for (size_t index = 0; index < 60; ++index)
			softwareRenderer.draw(drawInputInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_triangles_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}
}