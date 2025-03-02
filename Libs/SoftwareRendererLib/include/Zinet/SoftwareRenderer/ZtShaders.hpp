#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include <vector>

namespace zt::software_renderer
{
	class ZINET_SOFTWARE_RENDERER_API VertexShader
	{
	public:

		using ProcessVertexCallableT = std::function<void(const VertexShader&, Vertex& vertex)>;

		ProcessVertexCallableT processVertex = nullptr;

		operator bool() const { return processVertex.operator bool(); }

	};

	class ZINET_SOFTWARE_RENDERER_API FragmentShader
	{
	public:

		std::vector<RenderTarget> textures;

		Color sourceColor;

		using ProcessFragmentCallableT = std::function<void(const FragmentShader&, Pixel& fragment)>;

		ProcessFragmentCallableT processFragment = nullptr;

		operator bool() const { return processFragment.operator bool(); }

	};

	struct ZINET_SOFTWARE_RENDERER_API ShaderProgram
	{
		VertexShader vertexShader;
		FragmentShader fragmentShader;
	};

	Color sampleTexture(const RenderTarget& texture, Vector2f uv) {
		const auto resolution = texture.getResolution();

		const Vector2i texturePixelCoords{
			std::min(std::max(std::int32_t(std::round(uv.x * resolution.x)), 0), resolution.x - 1),
			std::min(std::max(std::int32_t(std::round(uv.y * resolution.y)), 0), resolution.y - 1)
		};

		return texture.getPixelColor(texturePixelCoords);
	}

}