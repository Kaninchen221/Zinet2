#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

namespace zt::software_renderer
{

	RenderTarget RenderTarget::Create(const Vector2ui& dimension)
	{
		RenderTarget renderTarget;
		renderTarget.dimension = dimension;

		renderTarget.texels.resize(dimension.x * dimension.y);

		return renderTarget;
	}

	RenderTarget RenderTarget::Create(const Vector2ui& dimension, const Texel& color)
	{
		RenderTarget renderTarget;
		renderTarget.dimension = dimension;

		renderTarget.texels.resize(dimension.x * dimension.y, color);

		return renderTarget;
	}

	Texel RenderTarget::getTexel(const Vector2ui& coords) const noexcept
	{
		const auto index = coordsToIndex(coords);

#	if ZINET_SANITY_CHECK
		if (index >= texels.size())
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

	void RenderTarget::setTexel(const Vector2ui& coords, const Texel& color) noexcept
	{
		const auto index = coordsToIndex(coords);

#	if ZINET_SANITY_CHECK
		if (index >= texels.size())
		{
			Logger->error("Attempted to set texel at out-of-bounds coordinates: ({}, {})", coords.x, coords.y);
			return;
		}
#	endif //ZINET_SANITY_CHECK

		texels[index] = color;
	}

}