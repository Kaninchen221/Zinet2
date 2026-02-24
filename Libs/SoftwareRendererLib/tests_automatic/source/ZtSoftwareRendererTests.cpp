#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtConstants.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	// TODO List:
	// - Draw Triangles
	// - Blending modes
	// - Cache simplified/optimized draw data
	class SoftwareRendererTests : public ::testing::Test
	{
	protected:

		static void FillVertices(Vertices& vertices)
		{
			const float min = 0.0f;
			vertices.push_back(Vertex{ Vector3f{ min, min, 0.0f }, Vector2f{ 0.0f, 0.0f }, Texel{ 255, 0, 0, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.8f, min, 0.0f }, Vector2f{ 1.0f, 0.0f }, Texel{ 0, 255, 0, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.8f, 0.8f, 0.0f }, Vector2f{ 1.0f, 1.0f }, Texel{ 0, 0, 255, 255 } });
			vertices.push_back(Vertex{ Vector3f{ min, 0.8f, 0.0f }, Vector2f{ 0.0f, 1.0f }, Texel{ 0, 0, 0, 255 } });
		}

		void SetUp() override
		{
			FillVertices(vertices);
			indices = { 0, 2, 1, 0, 3, 2 };
			renderTarget = RenderTarget::Create(SmallDimension, WhiteColor);

			drawData = 
			{
				.vertices = &vertices,
				.indices = &indices,
				.drawMode = DrawMode::Points,
				.linesColor = &linesColor
			}; 
			
			auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
			
			resultFilePath = FolderPath / (std::string(testInfo->test_suite_name()) + "_" + testInfo->name() + PNGExt);
		}

		void TearDown() override
		{
			ASSERT_TRUE(renderTarget.saveToPNG(resultFilePath));

			if constexpr (SoftwareRenderer::GetStatsEnabled())
			{
				renderer.logStats();
			}
		}

		inline static auto FolderPath = core::Paths::CurrentProjectRootPath() / "results";
		inline static auto PNGExt = std::string(".png");

		SoftwareRenderer renderer;

		Vertices vertices;
		Indices indices;
		RenderTarget renderTarget;
		Texel linesColor = BlueColor;
		DrawData drawData;
		core::Path resultFilePath;
	};

	TEST_F(SoftwareRendererTests, DrawPoints)
	{
		drawData.drawMode = DrawMode::Points;

		renderer.submitDrawData(&drawData);
		renderer.draw(&renderTarget);

		// Verify that the expected points were drawn with the correct colors
		for (const auto index : indices)
		{
			const auto& vertex = vertices[index];
			const auto& expectedColor = vertex.color;
			const auto coords = Vector2ui
			{
				static_cast<uint32_t>(vertex.position.x * renderTarget.getDimension().x),
				static_cast<uint32_t>(vertex.position.y * renderTarget.getDimension().y)
			};
			const auto actualColor = renderTarget.getTexel(coords);
			EXPECT_EQ(expectedColor, actualColor);
		}
	}

	TEST_F(SoftwareRendererTests, DrawLines)
	{
		drawData.drawMode = DrawMode::TriangleLines;

		renderer.submitDrawData(&drawData);
		renderer.draw(&renderTarget);
	}

	TEST_F(SoftwareRendererTests, DrawTriangles)
	{
		drawData.drawMode = DrawMode::Triangles;

		for (size_t i = 0; i < 50'000; i++) // We draw 2 triangles, so 50k iterations will draw 100k triangles
			renderer.submitDrawData(&drawData);

		renderer.draw(&renderTarget);
	}

	TEST(SoftwareRendererTest, IsAvailable)
	{
		ASSERT_TRUE(SoftwareRenderer::IsAvailable());
	}

	TEST(SoftwareRendererTest, StatsEnabled)
	{
#if ZINET_SOFTWARE_RENDERER_STATS
		ASSERT_TRUE(SoftwareRenderer::GetStatsEnabled());
#else
		ASSERT_FALSE(SoftwareRenderer::GetStatsEnabled());
#endif // ZINET_DEBUG
	}
}
