#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Math/ZtMath.hpp"

#include "Zinet/Core/ZtClock.hpp"

#include <utility>
#include <cmath>
#include <algorithm>

namespace zt::software_renderer
{
	void SoftwareRenderer::drawRenderTarget(const DrawRenderTargetInfo& drawInfo, RenderTarget& renderTarget)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif
		const Vector2i firstDestPixelCoords = {
			std::max(0, std::min(drawInfo.position.x, renderTarget.getResolution().x)),
			std::max(0, std::min(drawInfo.position.y, renderTarget.getResolution().y))
		};

		const Vector2i lastPixel = {
			std::clamp(drawInfo.srcRenderTarget.getResolution().x + drawInfo.position.x, 0, std::min(renderTarget.getResolution().x - drawInfo.position.x, drawInfo.srcRenderTarget.getResolution().x)),
			std::clamp(drawInfo.srcRenderTarget.getResolution().y + drawInfo.position.y, 0, std::min(renderTarget.getResolution().y - drawInfo.position.y, drawInfo.srcRenderTarget.getResolution().y))
		};

		// TODO: Optimize it or remove this method
		for (std::int32_t x = 0; x < lastPixel.x; x++)
		{
			for (std::int32_t y = 0; y < lastPixel.y; y++)
			{
				const Vector2i srcPixelCoords = { x, y };
				const Vector2i destPixelCoords = firstDestPixelCoords + srcPixelCoords;

				const Color& srcColor = drawInfo.srcRenderTarget.getPixelColor(srcPixelCoords);
				renderTarget.writePixel({ .coords = destPixelCoords, .color = srcColor });
			}
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Draw (renderTarget) took: {} milliseconds", elapsedTime);
#endif
	}

	void SoftwareRenderer::draw(DrawInfo& drawInfo, RenderTarget& renderTarget)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
		const bool cachedDraw = !drawInfo.isDirty;
#endif

		if (drawInfo.isDirty)
		{
#if ZINET_DEBUG
			if (!validateDrawInfo(drawInfo))
				return;
#endif
			drawInfo.cachedPixels.clear();

			// Input Assembler
			// ?

			vertexShader(drawInfo.vertices, drawInfo.shaderProgram);

			// Tessellation
			// ?

			// Geometry Shader
			// ?

			rasterization(drawInfo, renderTarget);

			fragmentShader(drawInfo, renderTarget);

			// Color Blending
			// ?

			drawInfo.isDirty = false;
			drawInfo.cachedPixels.shrink_to_fit();
		}

		writePixels(drawInfo, drawInfo.cachedPixels, renderTarget);

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Draw (drawInfo) took: {} milliseconds. Cached draw: {}", elapsedTime, cachedDraw);
#endif
	}

	bool SoftwareRenderer::validateDrawInfo(const DrawInfo& drawInfo) const
	{
		if ((drawInfo.drawMode == DrawMode::Triangles || drawInfo.drawMode == DrawMode::TrianglesLines) &&
			drawInfo.indices.size() % 3 != 0)
		{
			Logger->error("Indices has invalid number of elements: {}", drawInfo.indices.size());
			return false;
		}

		if (drawInfo.drawMode == DrawMode::Lines && drawInfo.indices.size() % 2 != 0)
		{
			Logger->error("Indices has invalid number of elements: {}", drawInfo.indices.size());
			return false;
		}

		if (drawInfo.vertices.empty())
		{
			Logger->error("Vertices is empty");
			return false;
		}

		return true;
	}

	void SoftwareRenderer::vertexShader(std::vector<Vertex>& vertices, const ShaderProgram& shaderProgram) const
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		auto& vertexShader = shaderProgram.vertexShader;
		if (vertexShader)
		{
			for (auto& vertex : vertices)
			{
				vertexShader.processVertex(vertexShader, vertex);
			}
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Vertex shader step took: {} milliseconds", elapsedTime);
#endif
	}

	void SoftwareRenderer::rasterization(DrawInfo& drawInfo, RenderTarget& renderTarget)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		std::vector<Pixel>& result = drawInfo.cachedPixels;

		if (drawInfo.drawMode == DrawMode::Points)
		{
			result.reserve(drawInfo.vertices.size());
			for (const auto& vertex : drawInfo.vertices)
			{
				Pixel& pixel = result.emplace_back();
				rasterizeVertexAsPoint(vertex, pixel, renderTarget);
			}
		}
		else if (drawInfo.drawMode == DrawMode::TrianglesLines)
		{
			for (size_t index = 0; index < drawInfo.indices.size(); index += 3)
			{
				const size_t firstIndex = drawInfo.indices[index];
				const size_t secondIndex = drawInfo.indices[index + 1];
				const size_t thirdIndex = drawInfo.indices[index + 2];

				const std::vector<Pixel> firstLinePixels = rasterizeLine(drawInfo.vertices[firstIndex], drawInfo.vertices[secondIndex], renderTarget);
				const std::vector<Pixel> secondLinePixels = rasterizeLine(drawInfo.vertices[secondIndex], drawInfo.vertices[thirdIndex], renderTarget);
				const std::vector<Pixel> thirdLinePixels = rasterizeLine(drawInfo.vertices[thirdIndex], drawInfo.vertices[firstIndex], renderTarget);

				result.reserve(result.size() + firstLinePixels.size() + secondLinePixels.size() + thirdLinePixels.size());
				result.insert(result.end(), firstLinePixels.begin(), firstLinePixels.end());
				result.insert(result.end(), secondLinePixels.begin(), secondLinePixels.end());
				result.insert(result.end(), thirdLinePixels.begin(), thirdLinePixels.end());
			}
		}
		else if (drawInfo.drawMode == DrawMode::Triangles)
		{
			for (size_t index = 0; index < drawInfo.indices.size(); index += 3)
			{
				const size_t firstIndex = drawInfo.indices[index];
				const size_t secondIndex = drawInfo.indices[index + 1];
				const size_t thirdIndex = drawInfo.indices[index + 2];

				const auto verticesSize = drawInfo.vertices.size();
				if (firstIndex >= verticesSize || secondIndex >= verticesSize || thirdIndex >= verticesSize)
				{
					Logger->error("Skip triangle with incdices: {}, {} and {}. Vertices size: {}",
						firstIndex, secondIndex, thirdIndex, verticesSize);
					continue;
				}

				const Triangle triangle
				{
					.v1 = drawInfo.vertices[firstIndex],
					.v2 = drawInfo.vertices[secondIndex],
					.v3 = drawInfo.vertices[thirdIndex]
				};
				barycentricFillTriangle(triangle, renderTarget, result);
			}
		}
		else if (drawInfo.drawMode == DrawMode::Lines)
		{
			for (size_t index = 0; index < drawInfo.indices.size(); index += 2)
			{
				const size_t firstIndex = drawInfo.indices[index];
				const size_t secondIndex = drawInfo.indices[index + 1];

				const std::vector<Pixel> pixels = rasterizeLine(drawInfo.vertices[firstIndex], drawInfo.vertices[secondIndex], renderTarget);

				result.reserve(result.size() + pixels.size());
				result.insert(result.end(), pixels.begin(), pixels.end());
			}
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Rasterization step took: {} milliseconds", elapsedTime);
#endif
	}

	void SoftwareRenderer::rasterizeVertexAsPoint(const Vertex& vertex, Pixel& pixel, const RenderTarget& renderTarget) const
	{
		const Vector2i pixelCoords = renderTarget.normalizedCoordsToPixelCoords(vertex.position);
		pixel.coords = pixelCoords;
		pixel.color = vertex.color;
	}

	std::vector<zt::software_renderer::Pixel> SoftwareRenderer::rasterizeLine(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const
	{
		// Bresenham's line algorithm

		std::vector<Pixel> result;

		const Vector2<std::int32_t> firstPoint = renderTarget.normalizedCoordsToPixelCoords(firstVertex.position);
		const Vector2<std::int32_t> secondPoint = renderTarget.normalizedCoordsToPixelCoords(secondVertex.position);

		const float distanceBetweenPoints = Math::Distance(firstPoint, secondPoint);

		auto plot = [&](const Vector2i& coords)
		{
			const float firstWeight = 1.f - Math::Distance(coords, firstPoint) / distanceBetweenPoints;
			const float secondWeight = 1.0f - firstWeight;

			Pixel& pixel = result.emplace_back();
			pixel.coords = coords;
			pixel.color = (firstVertex.color * firstWeight) + (secondVertex.color * secondWeight);

			//Logger->info("Coords: {}, {}", coords.x, coords.y);
			//Logger->info("First weight: {}", firstWeight);
			//Logger->info("Second weight: {}", secondWeight);
		};

		std::int32_t dx = std::abs(secondPoint.x - firstPoint.x);
		std::int32_t dy = std::abs(secondPoint.y - firstPoint.y);

		std::int32_t sx = (firstPoint.x < secondPoint.x) ? 1 : -1;
		std::int32_t sy = (firstPoint.y < secondPoint.y) ? 1 : -1;

		std::int32_t err = dx - dy;

		Vector2<std::int32_t> currentPoint = firstPoint;

		while (true) {
			plot(currentPoint);

			if (currentPoint.x == secondPoint.x && currentPoint.y == secondPoint.y) break;

			std::int32_t e2 = 2 * err;

			if (e2 > -dy) {
				err -= dy;
				currentPoint.x += sx;
			}
			if (e2 < dx) {
				err += dx;
				currentPoint.y += sy;
			}
		}

		return result;
	}

	std::vector<Pixel> SoftwareRenderer::rasterizeLineAntialiasing(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const
	{
		// Xiaolin Wu's line algorithm
		// Source: https://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm

		// Not working :(

		std::vector<Pixel> result;

		Vector2<std::int32_t> firstPoint = renderTarget.normalizedCoordsToPixelCoords(firstVertex.position);
		Vector2<std::int32_t> secondPoint = renderTarget.normalizedCoordsToPixelCoords(secondVertex.position);

		const float distanceBetweenPoints = Math::Distance(firstPoint, secondPoint);

		auto plot = [&](int x, int y, float brightess) {
			Pixel& pixel = result.emplace_back();
			pixel.coords = Vector2i(x, y);

			const float firstWeight = 1.f - Math::Distance({ x, y }, firstPoint) / distanceBetweenPoints;
			const float secondWeight = 1.0f - firstWeight;

			pixel.color = (firstVertex.color * firstWeight) + (secondVertex.color * secondWeight);
		};

		auto ipart = [](float x) -> int { return int(std::floor(x)); };
		auto round = [](float x) -> float { return std::round(x); };
		auto fpart = [](float x) -> float { return x - std::floor(x); };
		auto rfpart = [=](float x) -> float { return 1 - fpart(x); };

		const bool steep = std::abs(secondPoint.y - firstPoint.y) > std::abs(secondPoint.x - firstPoint.x);
		if (steep) {
			std::swap(firstPoint.x, firstPoint.y);
			std::swap(secondPoint.x, secondPoint.y);
		}
		if (firstPoint.x > secondPoint.x) {
			std::swap(firstPoint.x, secondPoint.x);
			std::swap(firstPoint.y, secondPoint.y);
		}

		const float dx = static_cast<float>(secondPoint.x - firstPoint.x);
		const float dy = static_cast<float>(secondPoint.y - firstPoint.y);
		const float gradient = (dx == 0) ? 1 : dy / dx;

		int xpx11;
		float intery;
		{
			const float xend = round(static_cast<float>(firstPoint.x));
			const float yend = firstPoint.y + gradient * (xend - firstPoint.x);
			const float xgap = rfpart(static_cast<float>(firstPoint.x + 0.5));
			xpx11 = int(xend);
			const int ypx11 = ipart(yend);
			if (steep) {
				plot(ypx11, xpx11, rfpart(yend) * xgap);
				plot(ypx11 + 1, xpx11, fpart(yend) * xgap);
			}
			else {
				plot(xpx11, ypx11, rfpart(yend) * xgap);
				plot(xpx11, ypx11 + 1, fpart(yend) * xgap);
			}
			intery = yend + gradient;
		}

		int xpx12;
		{
			const float xend = round(static_cast<float>(secondPoint.x));
			const float yend = secondPoint.y + gradient * (xend - secondPoint.x);
			const float xgap = rfpart(static_cast<float>(secondPoint.x + 0.5));
			xpx12 = int(xend);
			const int ypx12 = ipart(yend);
			if (steep) {
				plot(ypx12, xpx12, rfpart(yend) * xgap);
				plot(ypx12 + 1, xpx12, fpart(yend) * xgap);
			}
			else {
				plot(xpx12, ypx12, rfpart(yend) * xgap);
				plot(xpx12, ypx12 + 1, fpart(yend) * xgap);
			}
		}

		if (steep) {
			for (int x = xpx11 + 1; x < xpx12; x++) {
				plot(ipart(intery), x, rfpart(intery));
				plot(ipart(intery) + 1, x, fpart(intery));
				intery += gradient;
			}
		}
		else {
			for (int x = xpx11 + 1; x < xpx12; x++) {
				plot(x, ipart(intery), rfpart(intery));
				plot(x, ipart(intery) + 1, fpart(intery));
				intery += gradient;
			}
		}

		return result;
	}

	void SoftwareRenderer::barycentricFillTriangle(const Triangle& triangle, const RenderTarget& renderTarget, std::vector<Pixel>& result)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif
		const auto p1 = renderTarget.normalizedCoordsToPixelCoords(triangle.v1.position);
		const auto p2 = renderTarget.normalizedCoordsToPixelCoords(triangle.v2.position);
		const auto p3 = renderTarget.normalizedCoordsToPixelCoords(triangle.v3.position);

		const auto& c1 = triangle.v1.color;
		const auto& c2 = triangle.v2.color;
		const auto& c3 = triangle.v3.color;

		const auto& uv1 = triangle.v1.uv;
		const auto& uv2 = triangle.v2.uv;
		const auto& uv3 = triangle.v3.uv;

		const std::int32_t minX = std::min({ p1.x, p2.x, p3.x });
		const std::int32_t maxX = std::max({ p1.x, p2.x, p3.x });
		const std::int32_t minY = std::min({ p1.y, p2.y, p3.y });
		const std::int32_t maxY = std::max({ p1.y, p2.y, p3.y });

		const double invArea = 1.f / float((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));

		for (std::int32_t py = minY; py < maxY; py++)
		{
			const std::int32_t diffP3YPY = p3.y - py;
			const std::int32_t diffP2YPY = p2.y - py;
			const std::int32_t diffP1YPY = p1.y - py;

			for (std::int32_t px = minX; px < maxX; px++)
			{
				const double alpha = ((p2.x - px) * diffP3YPY - diffP2YPY * (p3.x - px)) * invArea;
				const double beta = ((p3.x - px) * diffP1YPY - diffP3YPY * (p1.x - px)) * invArea;
				const double gamma = 1.f - alpha - beta;

				const float nearlyZero = -1e-6f;

				if (alpha >= 0.f && beta >= 0.f && gamma >= nearlyZero)
				{
					result.push_back(
						Pixel {
							.coords = { px, py },
							.color = {
								static_cast<std::uint8_t>(alpha * c1.r + beta * c2.r + gamma * c3.r),
								static_cast<std::uint8_t>(alpha * c1.g + beta * c2.g + gamma * c3.g),
								static_cast<std::uint8_t>(alpha * c1.b + beta * c2.b + gamma * c3.b),
								static_cast<std::uint8_t>(alpha * c1.a + beta * c2.a + gamma * c3.a)
							},
							.uv = {
								alpha * uv1.x + beta * uv2.x + gamma * uv3.x,
								alpha * uv1.y + beta * uv2.y + gamma * uv3.y
							}
						}
					);
				}
			}
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Barycentric fill triangle took: {} milliseconds", elapsedTime);
#endif
	}

	void SoftwareRenderer::fragmentShader(DrawInfo& drawInfo, RenderTarget& renderTarget)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		auto fragmentShader = drawInfo.shaderProgram.fragmentShader;
		if (!fragmentShader)
			return;
		
		for (Pixel& pixel : drawInfo.cachedPixels)
		{
			if (!renderTarget.areCoordsValid(pixel.coords))
				continue;

			Color* sourceColor = renderTarget.getPixelColorAddr(pixel.coords);
			fragmentShader.sourceColor = *sourceColor;
			fragmentShader.processFragment(fragmentShader, pixel);
			*sourceColor = pixel.color;
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Fragment shader step took: {} milliseconds", elapsedTime);
#endif
	}

	void SoftwareRenderer::writePixels(const DrawInfo& drawInfo, std::vector<Pixel>& pixels, RenderTarget& renderTarget)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		for (const Pixel& pixel : drawInfo.cachedPixels)
		{
			if (!renderTarget.areCoordsValid(pixel.coords))
				continue;

			Color* sourceColor = renderTarget.getPixelColorAddr(pixel.coords);
			*sourceColor = pixel.color;
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Write pixels took: {} milliseconds", elapsedTime);
#endif
	}

}