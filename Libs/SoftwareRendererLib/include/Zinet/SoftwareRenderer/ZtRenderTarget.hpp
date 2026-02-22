#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtTexel.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include <cstdint>
#include <vector>

namespace zt::software_renderer
{
	class ZINET_SOFTWARE_RENDERER_API RenderTarget
	{
		inline static auto Logger = core::ConsoleLogger::Create("RenderTarget");

	public:

		RenderTarget() = default;

		RenderTarget(const RenderTarget&) = default;
		RenderTarget(RenderTarget&&) = default;

		RenderTarget& operator=(const RenderTarget&) = default;
		RenderTarget& operator=(RenderTarget&&) = default;

		~RenderTarget() = default;

		static RenderTarget Create(const Vector2ui& dimension);

		static RenderTarget Create(const Vector2ui& dimension, const Texel& color);

		auto& getDimension() const noexcept { return dimension; }

		size_t getSize() const noexcept { return sizeof(Texel) * texels.size(); }

		auto& getTexels() const noexcept { return texels; }

		Texel getTexel(const Vector2ui& coords) const noexcept;

		bool allTexelsAre(const Texel& texel) const noexcept;

		size_t coordsToIndex(const Vector2ui& coords) const noexcept { return coords.y * dimension.x + coords.x; }

		void clear(const Texel& clearColor) noexcept;

		void setTexel(const Vector2ui& coords, const Texel& color) noexcept;

		bool saveToPNG(const core::Path& filePath) const noexcept;

	private:

		Vector2ui dimension{};

		std::vector<Texel> texels{};

	};

}