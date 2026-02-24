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

	void SoftwareRenderer::submitDrawData(DrawData* drawData)
	{
#	if ZINET_SANITY_CHECK
		if (!drawData->IsValid()) ZINET_UNLIKELY
		{
			Logger->error("Invalid draw data provided to SoftwareRenderer::draw");
			return;
		}
#	endif // ZINET_SANITY_CHECK

		submittedDrawData.push_back(drawData);
	}

	void SoftwareRenderer::draw(RenderTarget* renderTarget)
	{
		currentRenderTarget = renderTarget;

		// TODO: Transform vertices

		for (const auto* drawDataPtr : submittedDrawData)
		{
			auto& drawData = *drawDataPtr;

			switch (drawData.drawMode)
			{
			case DrawMode::Triangles:
				rasterizeTriangles(drawData);
				break;

			case DrawMode::TriangleLines:
				rasterizeTriangleLines(drawData);
				break;

			case DrawMode::Points:
				rasterizePoints(drawData);
				break;

			default:
				Logger->warn("Unknown draw mode specified in DrawData");
				break;
			}
		}
	}

	void SoftwareRenderer::rasterizePoints(const DrawData& drawData)
	{
		// Get unique indices to avoid drawing the same point multiple times
		auto indices = *drawData.indices;
		std::sort(indices.begin(), indices.end());
		auto last = std::unique(indices.begin(), indices.end());
		indices.erase(last, indices.end());

		// Draw each point
		const auto& vertices = *drawData.vertices;
		auto& renderTarget = *currentRenderTarget;

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

			setTexel(position, color, renderTarget);
		}

		if constexpr (StatsEnabled)
		{
			pointsRasterized += indices.size();
		}
	}

	void SoftwareRenderer::rasterizeTriangleLines(const DrawData& drawData)
	{
		auto& indices = *drawData.indices;
		auto& vertices = *drawData.vertices;
		auto& renderTarget = *currentRenderTarget;
		auto& linesColor = drawData.linesColor;

		// Search for duplicated lines
		Indices simplifiedIndices; // TODO: Nice candidate for caching
		for (size_t i = 0; i < indices.size(); ++i)
		{
			const size_t nextIndex = (i + 1) % indices.size();

			const auto index1 = indices[i];
			const auto index2 = indices[nextIndex];

			bool addIndices = true;
			if (!simplifiedIndices.empty())
			{
				for (size_t siIndex = 0; siIndex < simplifiedIndices.size() - 1; siIndex += 2)
				{
					auto si1 = simplifiedIndices[siIndex];
					auto si2 = simplifiedIndices[siIndex + 1];
					if ((index1 == si1 && index2 == si2) || (index1 == si2 && index2 == si1))
					{
						addIndices = false;
						break;
					}
				}
			}

			if (addIndices)
			{
				simplifiedIndices.push_back(index1);
				simplifiedIndices.push_back(index2);
			}
		}

		// Draw lines
		for (size_t i = 0; i < simplifiedIndices.size() - 1; i += 2)
		{
			const auto index1 = simplifiedIndices[i];
			const auto index2 = simplifiedIndices[i + 1];

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

			rasterizeLine(data);
		}
	}

	void SoftwareRenderer::rasterizeLine(const LineAlgorithmData& data) noexcept
	{
		auto& position1 = data.position1;
		auto& position2 = data.position2;
		auto& renderTarget = *currentRenderTarget;
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
			setTexel(position, color, renderTarget);

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

		if constexpr (StatsEnabled)
		{
			++linesRasterized;
		}
	}

	void SoftwareRenderer::rasterizeTriangles(const DrawData& drawData)
	{
		auto& indices = *drawData.indices;
		auto& vertices = *drawData.vertices;
		auto& renderTarget = *currentRenderTarget;

		for (size_t i = 0; i < indices.size() - 2; i += 3)
		{
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];

			rasterizeTriangle({ &v0, &v1, &v2, &renderTarget });
		}
	}

	void SoftwareRenderer::rasterizeTriangle(const DrawTriangleData& data)
	{
		// TODO: Simplify and optimize it

		auto edgeFunction = [](const Vector3f& v0, const Vector3f& v1, const Vector3f& v2) -> float
		{
			return (v2.x - v0.x) * (v1.y - v0.y) - (v2.y - v0.y) * (v1.x - v0.x);
		};

		auto& renderTarget = *currentRenderTarget;

		auto& v0 = *data.v0;
		auto& v1 = *data.v1;
		auto& v2 = *data.v2;

		auto& pos0 = v0.position;
		auto& pos1 = v1.position;
		auto& pos2 = v2.position;

		//const float area = edgeFunction(pos0, pos1, pos2);

		const auto& dimension = renderTarget.getDimension();

		Vector2f boundsMinScalar;
		Vector2f boundsMaxScalar;

		boundsMinScalar.x = std::min({ pos0.x, pos1.x, pos2.x });
		boundsMinScalar.y = std::min({ pos0.y, pos1.y, pos2.y });

		boundsMaxScalar.x = std::max({ pos0.x, pos1.x, pos2.x });
		boundsMaxScalar.y = std::max({ pos0.y, pos1.y, pos2.y });

		boundsMinScalar.x = std::max(0.f, boundsMinScalar.x);
		boundsMinScalar.y = std::max(0.f, boundsMinScalar.y);

		boundsMaxScalar.x = std::min(static_cast<float>(renderTarget.getDimension().x - 1), boundsMaxScalar.x);
		boundsMaxScalar.y = std::min(static_cast<float>(renderTarget.getDimension().y - 1), boundsMaxScalar.y);

		const Vector2i boundsMin{ boundsMinScalar.x * dimension.x, boundsMinScalar.y * dimension.y };
		const Vector2i boundsMax{ boundsMaxScalar.x * dimension.x, boundsMaxScalar.y * dimension.y };

		// 1. Convert everything to PIXELS immediately
		float x0 = pos0.x * dimension.x; float y0 = pos0.y * dimension.y;
		float x1 = pos1.x * dimension.x; float y1 = pos1.y * dimension.y;
		float x2 = pos2.x * dimension.x; float y2 = pos2.y * dimension.y;

		// 2. Define the Edge Function constants (The Pineda "A, B, C")
		// E(x, y) = (y0 - y1)x + (x1 - x0)y + (x0y1 - y0x1)
		float a01 = y0 - y1; float b01 = x1 - x0; float c01 = x0 * y1 - y0 * x1;
		float a12 = y1 - y2; float b12 = x2 - x1; float c12 = x1 * y2 - y1 * x2;
		float a20 = y2 - y0; float b20 = x0 - x2; float c20 = x2 * y0 - y2 * x0;

		// 3. Evaluate at the center of the first pixel in the bounding box
		float startX = (float)boundsMin.x + 0.5f;
		float startY = (float)boundsMin.y + 0.5f;

		float w01_row = a01 * startX + b01 * startY + c01;
		float w12_row = a12 * startX + b12 * startY + c12;
		float w20_row = a20 * startX + b20 * startY + c20;

		for (int32_t y = boundsMin.y; y < boundsMax.y; ++y) {
			float w01 = w01_row;
			float w12 = w12_row;
			float w20 = w20_row;

			for (int32_t x = boundsMin.x; x < boundsMax.x; ++x) {
				// Use the winding-independent check
				if ((w01 >= 0 && w12 >= 0 && w20 >= 0) || (w01 <= 0 && w12 <= 0 && w20 <= 0)) {
					setTexel({ x, y }, BlueColor, renderTarget);
				}
				w01 += a01;
				w12 += a12;
				w20 += a20;
			}
			w01_row += b01;
			w12_row += b12;
			w20_row += b20;
		}

		if constexpr (StatsEnabled)
		{
			++trianglesRasterized;
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

	void SoftwareRenderer::logStats() const noexcept
	{
		if constexpr (StatsEnabled)
		{
			const auto info =
				fmt::format("SoftwareRenderer Stats:\n"""
					"Texels Rasterized : {}\n"
					"Points Rasterized : {}\n"
					"Lines Rasterized : {}\n"
					"Triangles Rasterized : {}",
					texelsRasterized, pointsRasterized, linesRasterized, trianglesRasterized);

			Logger->info(info);
		}
	}

}