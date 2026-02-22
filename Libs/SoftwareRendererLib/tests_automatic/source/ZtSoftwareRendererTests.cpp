#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtConstants.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	class SoftwareRendererTests : public ::testing::Test
	{
	protected:

		static void FillVertices(Vertices& vertices)
		{
			vertices.push_back(Vertex{ Vector3f{ 0.2f, 0.2f, 0.0f }, Vector2f{ 0.0f, 0.0f }, Texel{ 255, 0, 0, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.8f, 0.2f, 0.0f }, Vector2f{ 1.0f, 0.0f }, Texel{ 0, 255, 0, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.8f, 0.8f, 0.0f }, Vector2f{ 1.0f, 1.0f }, Texel{ 0, 0, 255, 255 } });
			vertices.push_back(Vertex{ Vector3f{ 0.2f, 0.8f, 0.0f }, Vector2f{ 0.0f, 1.0f }, Texel{ 255, 255, 255, 255 } });
		}

	};

	TEST_F(SoftwareRendererTests, IsAvailable)
	{
		ASSERT_TRUE(SoftwareRenderer::IsAvailable());
	}

	TEST_F(SoftwareRendererTests, DrawPoints)
	{
		SoftwareRenderer renderer;

		Vertices vertices;
		FillVertices(vertices);

		const Indices indices{ 0, 1, 2, 0, 2, 3 };

		auto renderTarget = RenderTarget::Create(TwoKDimension);

		const DrawData drawData
		{
			.vertices = &vertices,
			.indices = &indices,
			.renderTarget = &renderTarget,
			.drawMode = DrawMode::Points
		};

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
}
