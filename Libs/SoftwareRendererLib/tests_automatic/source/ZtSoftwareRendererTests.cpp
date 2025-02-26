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

		DrawInputInfo getInputDrawInfoRect() const
		{
			return DrawInputInfo
			{
				// (0,0) point is in upper left corner
				.drawMode = DrawMode::Points,
				.antialiasing = false,
				.vertices = std::vector<Vertex>{
					Vertex{ { .30f, .25f }, RedColor },
					Vertex{ { .80f, .25f }, GreenColor },
					Vertex{ { .25f, .75f }, BlueColor },
					Vertex{ { .75f, .75f }, BlackColor }
				},
				.indices = {
					0, 1, 2,
					1, 3, 2
				},
				.vertexShader = VertexShader{}
			};
		}

		void createRenderTarget(RenderTarget& renderTarget, Color fillColor = WhiteColor)
		{
			const Vector2ui size = { 1920, 1080 };
			const ColorFormat colorFormat = ColorFormat::R8G8B8A8_SRGB;
			bool renderTargetCreateResult = renderTarget.createEmpty(size, colorFormat);
			ASSERT_TRUE(renderTargetCreateResult);
			renderTarget.fill(fillColor);
		}
	};

	TEST_F(SoftwareRendererTests, DrawPointsTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		softwareRenderer.draw(getInputDrawInfoRect(), renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_points_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawLinesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		auto drawInfo = getInputDrawInfoRect();
		drawInfo.drawMode = DrawMode::Lines;
		drawInfo.antialiasing = false;
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_lines_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}

	TEST_F(SoftwareRendererTests, DrawTrianglesTest)
	{
		RenderTarget renderTarget;
		createRenderTarget(renderTarget);

		auto drawInfo = getInputDrawInfoRect();
		drawInfo.drawMode = DrawMode::Triangles;
		drawInfo.antialiasing = false;
		softwareRenderer.draw(drawInfo, renderTarget);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "software_renderer_draw_triangles_result.png";
		const bool saveResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveResult);
	}
}