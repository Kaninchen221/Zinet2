#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/SoftwareRenderer/ZtVertex.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"
#include "Zinet/SoftwareRenderer/ZtTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#if ZINET_DEBUG
#	define ZINET_SOFTWARE_RENDERER_STATS 1
#else
#	define ZINET_SOFTWARE_RENDERER_STATS 0
#endif // ZINET_DEBUG


namespace zt::software_renderer
{
	enum class DrawMode
	{
		Triangles,
		TriangleLines,
		Points
	};

	struct DrawData
	{
		const Vertices* vertices{};
		const Indices* indices{};
		DrawMode drawMode = DrawMode::Triangles;

		Texel* linesColor{}; // Used only for lines drawing mode

		constexpr bool IsValid() const noexcept
		{
			return vertices && indices && 
				(drawMode != DrawMode::Points || linesColor);
		}
	};

	class ZINET_SOFTWARE_RENDERER_API SoftwareRenderer
	{
		inline static auto Logger = core::ConsoleLogger::Create("SoftwareRenderer");

#if ZINET_SOFTWARE_RENDERER_STATS
		constexpr static bool StatsEnabled = true;
#else
		constexpr static bool StatsEnabled = false;
#endif // ZINET_DEBUG

	public:

		static bool IsAvailable() noexcept;

		constexpr static bool GetStatsEnabled() noexcept { return StatsEnabled; }

		// Submitted draw data must exists until the draw call finishes
		void submitDrawData(DrawData* drawData);

		void draw(RenderTarget* renderTarget);

	private:

		void rasterizePoints(const DrawData& drawData);

		void rasterizeTriangleLines(const DrawData& drawData);

		struct LineAlgorithmData
		{
			Vector2i position1;
			Vector2i position2;
			RenderTarget* renderTarget;
			Texel* color;
		};

		void rasterizeLine(const LineAlgorithmData& data) noexcept;

		void rasterizeTriangles(const DrawData& drawData);

		struct DrawTriangleData
		{
			const Vertex* v0;
			const Vertex* v1;
			const Vertex* v2;
			RenderTarget* renderTarget;
		};

		void rasterizeTriangle(const DrawTriangleData& data);

		inline Vector2i normalizedToRenderTarget(const Vertex& vertex, const Vector2i& renderTargetDimension) const noexcept;

		std::vector<DrawData*> submittedDrawData;

		RenderTarget* currentRenderTarget{};

		void setTexel(const Vector2i& position, const Texel& color, RenderTarget& renderTarget) noexcept;

		// Stats
		size_t pointsRasterized = 0;
		size_t linesRasterized = 0;
		size_t trianglesRasterized = 0;

		size_t texelsRasterized = 0;

	public:
		void logStats() const noexcept;
	private:

	};
}