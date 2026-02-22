#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtFile.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace zt::software_renderer
{
	RenderTarget RenderTarget::Create(const Vector2i& dimension)
	{
		if (dimension.x < 1 || dimension.y < 1)
		{
			Logger->error("Invalid render target dimension: ({}, {})", dimension.x, dimension.y);
			return {};
		}

		RenderTarget renderTarget;
		renderTarget.dimension = dimension;

		renderTarget.texels.resize(dimension.x * dimension.y);

		return renderTarget;
	}

	RenderTarget RenderTarget::Create(const Vector2i& dimension, const Texel& color)
	{
		if (dimension.x < 1 || dimension.y < 1)
		{
			Logger->error("Invalid render target dimension: ({}, {})", dimension.x, dimension.y);
			return {};
		}

		RenderTarget renderTarget;
		renderTarget.dimension = dimension;

		renderTarget.texels.resize(dimension.x * dimension.y, color);

		return renderTarget;
	}

	Texel RenderTarget::getTexel(const Vector2i& coords) const noexcept
	{
		const auto index = coordsToIndex(coords);

#	if ZINET_SANITY_CHECK
		if (index >= texels.size()) ZINET_UNLIKELY
		{
			Logger->error("Attempted to get texel at out-of-bounds coordinates: ({}, {})", coords.x, coords.y);
			return {};
		}
#	endif //ZINET_SANITY_CHECK

		return texels[index];
	}

	bool RenderTarget::allTexelsAre(const Texel& texel) const noexcept
	{
		for (const auto& srcTexel : texels)
		{
			if (srcTexel != texel)
				return false;
		}

		return true;
	}

	void RenderTarget::clear(const Texel& clearColor) noexcept
	{
		std::fill(texels.begin(), texels.end(), clearColor);
	}

	void RenderTarget::setTexel(const Vector2i& coords, const Texel& color) noexcept
	{
		const auto index = coordsToIndex(coords);

#	if ZINET_SANITY_CHECK
		if (index >= texels.size()) ZINET_UNLIKELY
		{
			Logger->error("Attempted to set texel at out-of-bounds coordinates: ({}, {})", coords.x, coords.y);
			return;
		}
#	endif //ZINET_SANITY_CHECK

		texels[index] = color;
	}

	bool RenderTarget::saveToPNG(const core::Path& filePath) const noexcept
	{
		const auto path = filePath.string();
		const int width = static_cast<int>(dimension.x);
		const int height = static_cast<int>(dimension.y);
		return stbi_write_png(path.c_str(), width, height, Texel::Channels, texels.data(), 0);
	}

}