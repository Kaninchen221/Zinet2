#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Math/ZtMath.hpp"

#include <utility>
#include <cmath>
#include <algorithm>

namespace zt::software_renderer
{
	void SoftwareRenderer::draw(DrawInputInfo drawInputInfo, RenderTarget& renderTarget)
	{
#if ZINET_DEBUG
		// Validate indices
		if (drawInputInfo.indices.size() % 3 != 0)
		{
			Logger->error("Indices has invalid number of elements: {}", drawInputInfo.indices.size());
			return;
		}
#endif

		// Input Assembler

		// Vertex Shader
		for (auto& vertex : drawInputInfo.vertices)
		{
			drawInputInfo.vertexShader.processVertex(vertex);
		}

		// Tessellation

		// Geometry Shader

		// Rasterization

		// Fragment Shader

		// Color Blending

		// Write to render target?

		std::vector<Pixel> pixels;

		if (drawInputInfo.drawMode == DrawMode::Points)
		{
			pixels.reserve(pixels.size());
			for (const auto& vertex : drawInputInfo.vertices)
			{
				Pixel& pixel = pixels.emplace_back();
				rasterizeVertexAsPoint(vertex, pixel, renderTarget);
			}
		}
		else if (drawInputInfo.drawMode == DrawMode::Lines || drawInputInfo.drawMode == DrawMode::Triangles)
		{
			for (size_t index = 0; index < drawInputInfo.indices.size(); index += 3)
			{
				const size_t firstIndex = drawInputInfo.indices[index];
				const size_t secondIndex = drawInputInfo.indices[index + 1];
				const size_t thirdIndex = drawInputInfo.indices[index + 2];

				const std::vector<Pixel> firstLinePixels = rasterizeLine(drawInputInfo.vertices[firstIndex], drawInputInfo.vertices[secondIndex], renderTarget);
				const std::vector<Pixel> secondLinePixels = rasterizeLine(drawInputInfo.vertices[secondIndex], drawInputInfo.vertices[thirdIndex], renderTarget);
				const std::vector<Pixel> thirdLinePixels = rasterizeLine(drawInputInfo.vertices[thirdIndex], drawInputInfo.vertices[firstIndex], renderTarget);

				pixels.append_range(firstLinePixels);
				pixels.append_range(secondLinePixels);
				pixels.append_range(thirdLinePixels);
			}
		}
		
		if (drawInputInfo.drawMode == DrawMode::Triangles)
		{
			for (size_t index = 0; index < drawInputInfo.indices.size(); index += 3)
			{
				const size_t firstIndex = drawInputInfo.indices[index];
				const size_t secondIndex = drawInputInfo.indices[index + 1];
				const size_t thirdIndex = drawInputInfo.indices[index + 2];

				std::vector<Pixel> fillPixels = barycentricFillTriangle(drawInputInfo.vertices[firstIndex], drawInputInfo.vertices[secondIndex], drawInputInfo.vertices[thirdIndex], renderTarget);
				pixels.append_range(fillPixels);
			}
		}

		renderTarget.writePixels(pixels);
	}

	void SoftwareRenderer::rasterizeVertexAsPoint(const Vertex& vertex, Pixel& pixel, const RenderTarget& renderTarget) const
	{
		const Vector2ui pixelCoords = renderTarget.normalizedCoordsToPixelCoords(vertex.position);
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

		auto plot = [&](const Vector2ui& coords)
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
			pixel.coords = Vector2ui(x, y);

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

	std::vector<Pixel> SoftwareRenderer::barycentricFillTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const RenderTarget& renderTarget)
	{
		std::vector<Pixel> result;

		const Vector2ui p1 = renderTarget.normalizedCoordsToPixelCoords(vertex1.position);
		const Vector2ui p2 = renderTarget.normalizedCoordsToPixelCoords(vertex2.position);
		const Vector2ui p3 = renderTarget.normalizedCoordsToPixelCoords(vertex3.position);

		const Color& c1 = vertex1.color;
		const Color& c2 = vertex2.color;
		const Color& c3 = vertex3.color;

		const std::uint32_t minX = std::min({ p1.x, p2.x, p3.x });
		const std::uint32_t maxX = std::max({ p1.x, p2.x, p3.x });
		const std::uint32_t minY = std::min({ p1.y, p2.y, p3.y });
		const std::uint32_t maxY = std::max({ p1.y, p2.y, p3.y });

		result.reserve(maxX * maxY);

		const float area = static_cast<float>((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));

		for (std::uint32_t py = minY; py <= maxY; py++)
		{
			for (std::uint32_t px = minX; px <= maxX; px++)
			{
				float alpha = ((p2.x - px) * (p3.y - py) - (p2.y - py) * (p3.x - px)) / area;
				float beta = ((p3.x - px) * (p1.y - py) - (p3.y - py) * (p1.x - px)) / area;
				float gamma = 1u - alpha - beta;

				if (alpha >= 0 && beta >= 0 && gamma >= 0) 
				{
					Color color;
					color.r = static_cast<std::uint8_t>(alpha * static_cast<std::uint32_t>(c1.r) + beta * static_cast<std::uint32_t>(c2.r) + gamma * static_cast<std::uint32_t>(c3.r));
					color.g = static_cast<std::uint8_t>(alpha * static_cast<std::uint32_t>(c1.g) + beta * static_cast<std::uint32_t>(c2.g) + gamma * static_cast<std::uint32_t>(c3.g));
					color.b = static_cast<std::uint8_t>(alpha * static_cast<std::uint32_t>(c1.b) + beta * static_cast<std::uint32_t>(c2.b) + gamma * static_cast<std::uint32_t>(c3.b));
					color.a = static_cast<std::uint8_t>(alpha * static_cast<std::uint32_t>(c1.a) + beta * static_cast<std::uint32_t>(c2.a) + gamma * static_cast<std::uint32_t>(c3.a));

					result.push_back(Pixel{ .coords = { px, py }, .color = color });
				}
			}
		}

		return result;
	}

}