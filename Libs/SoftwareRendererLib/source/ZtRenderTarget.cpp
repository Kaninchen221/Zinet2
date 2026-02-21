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

}