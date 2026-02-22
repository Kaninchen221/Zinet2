#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtConstants.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	// TODO List:
	// - Draw Triangles
	// - Blending modes
	class SoftwareRendererTests : public ::testing::Test
	{
	protected:

		static void FillVertices(Vertices& vertices)
		{
			vertices.push_back(Vertex{ Vector3f{ 0.2f, 0.2f, 0.0f }, Vector2f{ 0.0f, 0.0f }, Texel{ 255, 0, 0, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.8f, 0.2f, 0.0f }, Vector2f{ 1.0f, 0.0f }, Texel{ 0, 255, 0, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.8f, 0.8f, 0.0f }, Vector2f{ 1.0f, 1.0f }, Texel{ 0, 0, 255, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.2f, 0.8f, 0.0f }, Vector2f{ 0.0f, 1.0f }, Texel{ 0, 0, 0, 255 } });
		}

		void SetUp() override
		{
			FillVertices(vertices);
			indices = { 0, 1, 2, 0, 2, 3 };
			renderTarget = RenderTarget::Create(SmallDimension, WhiteColor);

			drawData = 
			{
				.vertices = &vertices,
				.indices = &indices,
				.renderTarget = &renderTarget,
				.drawMode = DrawMode::Points,
				.linesColor = &linesColor
			}; 
			
			auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();

			resultFilePath = FolderPath / (std::string(testInfo->test_suite_name()) + "_" + testInfo->name() + PNGExt);
		}

		void TearDown() override
		{
			ASSERT_TRUE(renderTarget.saveToPNG(resultFilePath));
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
		renderer.draw(drawData);

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
		indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
		drawData.drawMode = DrawMode::Lines;

		renderer.draw(drawData);
	}

	TEST(SoftwareRendererTest, IsAvailable)
	{
		ASSERT_TRUE(SoftwareRenderer::IsAvailable());
	}
}
