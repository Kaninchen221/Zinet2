#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

#include "Zinet/SoftwareRenderer/ZtDirectXMathSingleHeader.hpp"
#include "Zinet/SoftwareRenderer/ZtConstants.hpp"

#include "Zinet/Math/ZtMath.hpp"

#include <algorithm>

namespace zt::software_renderer
{
	bool SoftwareRenderer::IsAvailable() noexcept
	{
		const auto IsDirectXMathSupported = DirectX::XMVerifyCPUSupport();
		return IsDirectXMathSupported;
	}

	void SoftwareRenderer::draw(const DrawData& drawData)
	{
#	if ZINET_DEBUG
		if (!drawData.IsValid()) ZINET_UNLIKELY
		{
			Logger->error("Invalid draw data provided to SoftwareRenderer::draw");
			return;
		}
#	endif

		switch (drawData.drawMode)
		{
		case DrawMode::Triangles:
			Logger->error("Draw triangles is not implemented");
			break;

		case DrawMode::Lines:
			drawLines(drawData);
			break;

		case DrawMode::Points:
			drawPoints(drawData);
			break;

		default:
			Logger->warn("Unknown draw mode specified in DrawData");
			break;
		}
	}

	void SoftwareRenderer::drawPoints(const DrawData& drawData)
	{
		// Get unique indices to avoid drawing the same point multiple times
		auto indices = *drawData.indices;
		std::sort(indices.begin(), indices.end());
		auto last = std::unique(indices.begin(), indices.end());
		indices.erase(last, indices.end());

		// Draw each point
		const auto& vertices = *drawData.vertices;
		auto& renderTarget = *drawData.renderTarget;

		for (const auto index : indices)
		{
#		if ZINET_SANITY_CHECK
			if (index >= vertices.size()) ZINET_UNLIKELY
			{
				Logger->warn("Index {} is out of bounds for vertices array", index);
				continue;
			}
#		endif // ZINET_SANITY_CHECK

 			const auto& vertex = vertices[index];
 			const auto& color = vertex.color;

			// Convert normalized coordinates to render target pixel coordinates
			const auto position = normalizedToRenderTarget(vertex, renderTarget.getDimension());

			renderTarget.setTexel({ position.x, position.y }, color);
		}
	}

	void SoftwareRenderer::drawLines(const DrawData& drawData)
	{
		auto& indices = *drawData.indices;
		auto& vertices = *drawData.vertices;
		auto& renderTarget = *drawData.renderTarget;
		auto& linesColor = drawData.linesColor;

		for (size_t i = 0; i + 1 < indices.size(); i += 2)
		{
			const size_t index1 = indices[i];
			const size_t index2 = indices[i + 1];

#		if ZINET_SANITY_CHECK
			if (index1 >= vertices.size() || index2 >= vertices.size()) ZINET_UNLIKELY
			{
				Logger->warn("Line indices {} and {} are out of bounds for vertices array", index1, index2);
				continue;
			}
#		endif // ZINET_SANITY_CHECK

			LineAlgorithmData data
			{
				.position1 = normalizedToRenderTarget(vertices[index1], renderTarget.getDimension()),
				.position2 = normalizedToRenderTarget(vertices[index2], renderTarget.getDimension()),
				.renderTarget = &renderTarget,
				.color = linesColor
			};

			lineAlgorithm(data);
		}
	}

	void SoftwareRenderer::lineAlgorithm(const LineAlgorithmData& data) const noexcept
	{
		auto& position1 = data.position1;
		auto& position2 = data.position2;
		auto& renderTarget = *data.renderTarget;
		auto& color = *data.color;

		auto position = position1;

		// Bresenham's line algorithm
		const int32_t disX = glm::abs(position2.x - position1.x);
		const int32_t disY = -glm::abs(position2.y - position1.y);

		const int32_t signX = position1.x < position2.x ? 1 : -1;
		const int32_t signY = position1.y < position2.y ? 1 : -1;

		int32_t err = disX + disY;

		while (true)
		{
			renderTarget.setTexel(position, color);

			int32_t e2 = 2 * err;

			// step x
			if (e2 >= disY)
			{
				if (position.x == position2.x) break;

				err += disY;
				position.x += signX;
			}

			// step y
			if (e2 <= disX)
			{
				if (position.y == position2.y) break;

				err += disX;
				position.y += signY;
			}
		}
	}

	Vector2i SoftwareRenderer::normalizedToRenderTarget(const Vertex& vertex, const Vector2i& renderTargetDimension) const noexcept
	{
		const auto x = static_cast<int32_t>(vertex.position.x * renderTargetDimension.x);
		const auto y = static_cast<int32_t>(vertex.position.y * renderTargetDimension.y);

		return Vector2i
		{
			glm::clamp(x, 0, renderTargetDimension.x - 1),
			glm::clamp(y, 0, renderTargetDimension.y - 1)
		};
	}

}