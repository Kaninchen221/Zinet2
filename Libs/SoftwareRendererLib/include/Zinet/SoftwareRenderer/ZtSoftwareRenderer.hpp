#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/SoftwareRenderer/ZtVertex.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"
#include "Zinet/SoftwareRenderer/ZtTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

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
		RenderTarget* renderTarget{};
		DrawMode drawMode = DrawMode::Triangles;

		Texel* linesColor{}; // Used only for lines drawing mode

		constexpr bool IsValid() const noexcept
		{
			return vertices != nullptr && indices != nullptr && renderTarget != nullptr;
		}
	};

	class ZINET_SOFTWARE_RENDERER_API SoftwareRenderer
	{
		inline static auto Logger = core::ConsoleLogger::Create("SoftwareRenderer");

	public:

		static bool IsAvailable() noexcept;

		void draw(const DrawData& drawData);

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

		void lineAlgorithm(const LineAlgorithmData& data) const noexcept;

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

	};

}