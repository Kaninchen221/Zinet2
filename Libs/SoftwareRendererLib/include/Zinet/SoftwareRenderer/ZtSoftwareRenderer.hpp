#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtTypes.hpp"
#include "Zinet/SoftwareRenderer/ZtShaders.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::software_renderer
{
	struct ZINET_SOFTWARE_RENDERER_API DrawRenderTargetInfo
	{
		const RenderTarget& srcRenderTarget;
		Vector2i position;
	};

	struct ZINET_SOFTWARE_RENDERER_API DrawInfo
	{
		DrawMode drawMode = DrawMode::Points;
		std::vector<Vertex> vertices;
		std::vector<size_t> indices;
		ShaderProgram shaderProgram;
		bool isDirty = true;
		std::vector<Pixel> cachedPixels;
	};

	class ZINET_SOFTWARE_RENDERER_API SoftwareRenderer
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("SoftwareRenderer");

	public:

		SoftwareRenderer() = default;
		SoftwareRenderer(const SoftwareRenderer& other) = default;
		SoftwareRenderer(SoftwareRenderer&& other) = default;

		SoftwareRenderer& operator = (const SoftwareRenderer& other) = default;
		SoftwareRenderer& operator = (SoftwareRenderer&& other) = default;

		~SoftwareRenderer() noexcept = default;

		void drawRenderTarget(const DrawRenderTargetInfo& drawInfo, RenderTarget& renderTarget);

		// TODO: Cache draw
		void draw(DrawInfo& drawInfo, RenderTarget& renderTarget);

	protected:

		bool validateDrawInfo(const DrawInfo& drawInfo) const;

		void vertexShader(std::vector<Vertex>& vertices, const ShaderProgram& shaderProgram) const;

		void rasterization(DrawInfo& drawInfo, RenderTarget& renderTarget);

		void rasterizeVertexAsPoint(const Vertex& vertex, Pixel& pixel, const RenderTarget& renderTarget) const;
		std::vector<Pixel> rasterizeLine(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const;
		std::vector<Pixel> rasterizeLineAntialiasing(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const;

		// The outputs are pixels filling the triangle
		void barycentricFillTriangle(const Triangle& triangle, const RenderTarget& renderTarget, std::vector<Pixel>& result);

		void fragmentShader(DrawInfo& drawInfo, RenderTarget& renderTarget);

		void writePixels(const DrawInfo& drawInfo, std::vector<Pixel>& pixels, RenderTarget& renderTarget);

	};
}