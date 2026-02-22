#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

#include "Zinet/SoftwareRenderer/ZtDirectXMathSingleHeader.hpp"
#include <algorithm>

namespace zt::software_renderer
{

	bool SoftwareRenderer::IsAvailable() noexcept
	{
		const auto IsDirectXMathSupported = DirectX::XMVerifyCPUSupport();
		return IsDirectXMathSupported;
	}

	void SoftwareRenderer::draw(const DrawData& drawData) noexcept
	{
#	if ZINET_DEBUG
		if (!drawData.IsValid()) [[unlikely]]
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
			Logger->error("Draw Lines is not implemented");
			break;

		case DrawMode::Points:
			drawPoints(drawData);
			break;

		default:
			Logger->warn("Unknown draw mode specified in DrawData");
			break;
		}


	}

	void SoftwareRenderer::drawPoints(const DrawData& drawData) noexcept
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
			if (index >= vertices.size())
			{
				Logger->warn("Index {} is out of bounds for vertices array", index);
				continue;
			}
#		endif // ZINET_SANITY_CHECK

 			const auto& vertex = vertices[index];
 			const auto& color = vertex.color;

 			// Convert normalized device coordinates to render target pixel coordinates
 			const auto x = static_cast<uint32_t>(vertex.position.x * renderTarget.getDimension().x);
			const auto y = static_cast<uint32_t>(vertex.position.y * renderTarget.getDimension().y);

#		if ZINET_SANITY_CHECK
 			if (x >= renderTarget.getDimension().x || y >= renderTarget.getDimension().y)
 			{
 				Logger->warn("Vertex at index {} has position ({}, {}) which is out of render target bounds", index, vertex.position.x, vertex.position.y);
 				continue;
			}
#		endif // ZINET_SANITY_CHECK

			renderTarget.setTexel({ x, y }, color);
		}
	}

}