#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include <vector>

namespace zt::software_renderer
{
	class ZINET_SOFTWARE_RENDERER_API VertexShader
	{
	public:

		Vector2f cameraPosition;
		Vector2f cameraSize;

		Vector2f position;
		Vector2f size;

		using ProcessVertexCallableT = std::function<void(const VertexShader&, Vertex& vertex)>;

		ProcessVertexCallableT processVertex = nullptr;

		operator bool() const { return processVertex.operator bool(); }

	};

	ZINET_SOFTWARE_RENDERER_API 
	void VertexShaderVerticesToWorldProcess(const VertexShader& vertexShader, Vertex& vertex);

	class ZINET_SOFTWARE_RENDERER_API FragmentShader
	{
	public:

		std::vector<RenderTarget> textures;

		Color* sourceColor;

		using ProcessFragmentCallableT = std::function<void(const FragmentShader&, Pixel& fragment)>;

		ProcessFragmentCallableT processFragment = nullptr;

		operator bool() const { return processFragment.operator bool(); }

	};

	struct ZINET_SOFTWARE_RENDERER_API ShaderProgram
	{
		VertexShader vertexShader;
		FragmentShader fragmentShader;
	};

	inline Color SampleTexture(const RenderTarget& texture, const Vector2f& uv) {
		const auto& resolution = texture.getResolution();

		const Vector2i texturePixelCoords {
			std::min<float>(std::max<float>(std::floor(uv.x * resolution.x), 0.f), resolution.x - 1.f),
			std::min<float>(std::max<float>(std::floor(uv.y * resolution.y), 0.f), resolution.y - 1.f)
		};

		return texture.getPixelColor(texturePixelCoords);
	}

	inline Color SampleTextureUnsafe(const RenderTarget& texture, const Vector2f& uv) {
		const auto& resolution = texture.getResolution();

		const Vector2i texturePixelCoords{
			uv.x * resolution.x,
			uv.y * resolution.y
		};

		return texture.getPixelColor(texturePixelCoords);
	}

	ZINET_SOFTWARE_RENDERER_API
	void FragmentShaderSampleTextureProcess(const FragmentShader& fragmentShader, Pixel& fragment);
}