#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::software_renderer
{
	class RenderTarget;

	struct ZINET_SOFTWARE_RENDERER_API ShaderProgram
	{
		VertexShader vertexShader;
		FragmentShader fragmentShader;
	};

	struct ZINET_SOFTWARE_RENDERER_API DrawInfo
	{
		DrawMode drawMode = DrawMode::Points;
		bool antialiasing = false;
		std::vector<Vertex> vertices;
		std::vector<size_t> indices;
		ShaderProgram shaderProgram;
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

		void draw(DrawInfo drawInputInfo, RenderTarget& renderTarget);

		// TODO: draw render target in render target

	protected:

		std::vector<Pixel> rasterization(DrawInfo& drawInputInfo, RenderTarget& renderTarget);

		void rasterizeVertexAsPoint(const Vertex& vertex, Pixel& pixel, const RenderTarget& renderTarget) const;
		std::vector<Pixel> rasterizeLine(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const;
		std::vector<Pixel> rasterizeLineAntialiasing(const Vertex& firstVertex, const Vertex& secondVertex, const RenderTarget& renderTarget) const;

		// The outputs are pixels filling the triangle
		std::vector<Pixel> barycentricFillTriangle(const Triangle& triangle, const RenderTarget& renderTarget);

		void writePixels(const DrawInfo& drawInputInfo, std::vector<Pixel>& pixels, RenderTarget& renderTarget);

	};
}