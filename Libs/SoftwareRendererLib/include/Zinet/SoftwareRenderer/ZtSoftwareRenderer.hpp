#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::software_renderer
{
	class RenderTarget;
	struct Pixel;

	enum class DrawMode
	{
		Points,
		Lines,
		Triangles
	};

	struct ZINET_SOFTWARE_RENDERER_API Vertex
	{
		Vector2f position;
		Vector4f color;
	};

	class ZINET_SOFTWARE_RENDERER_API VertexShader
	{
	public:

		void processVertex(Vertex& vertex) const { }

	};

	struct ZINET_SOFTWARE_RENDERER_API DrawInputInfo
	{
		DrawMode drawMode = DrawMode::Points;
		bool antialiasing = false;
		std::vector<Vertex> vertices;
		std::vector<size_t> indices;
		VertexShader vertexShader;
	};

	class ZINET_SOFTWARE_RENDERER_API SoftwareRenderer
	{
	protected:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("SoftwareRenderer");

	public:

		SoftwareRenderer() = default;
		SoftwareRenderer(const SoftwareRenderer& other) = default;
		SoftwareRenderer(SoftwareRenderer&& other) = default;

		SoftwareRenderer& operator = (const SoftwareRenderer& other) = default;
		SoftwareRenderer& operator = (SoftwareRenderer&& other) = default;

		~SoftwareRenderer() noexcept = default;

		void draw(DrawInputInfo drawInputInfo, RenderTarget& renderTarget);

	protected:

		void rasterization(DrawInputInfo& drawInputInfo, RenderTarget& renderTarget);

		void rasterizeVertexAsPoint(const Vertex& vertex, Pixel& pixel, const RenderTarget& renderTarget) const;
		std::vector<Pixel> rasterizeLine(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const;
		std::vector<Pixel> rasterizeLineAntialiasing(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const;

		// The outputs are pixels filling the triangle
		std::vector<Pixel> barycentricFillTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const RenderTarget& renderTarget);

	};
}