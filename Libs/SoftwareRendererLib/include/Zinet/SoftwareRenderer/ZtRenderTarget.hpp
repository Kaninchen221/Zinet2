#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtTexel.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <cstdint>
#include <vector>

namespace zt::software_renderer
{

	class ZINET_SOFTWARE_RENDERER_API RenderTarget
	{

		RenderTarget() = default;

	public:

		RenderTarget(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = default;

		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget& operator=(RenderTarget&&) = default;

		~RenderTarget() = default;

		static RenderTarget Create(const Vector2ui& dimension);

		static RenderTarget Create(const Vector2ui& dimension, const Texel& color);

		auto& getDimension() const noexcept { return dimension; }

		size_t getSize() const noexcept { return sizeof(Texel) * texels.size(); }

		auto& getTexels() const noexcept { return texels; }

		bool allTexelsAre(const Texel& texel) const noexcept;

		void clear(const Texel& clearColor) noexcept;

	private:

		Vector2ui dimension{};

		std::vector<Texel> texels{};

	};

}