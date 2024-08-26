#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtTypes.hpp"

#include <utility>

namespace zt::rhi::opengl
{

	class ZINET_RHI_API Viewport
	{
	public:
		Viewport() = default;
		Viewport(const Viewport& other) = default;
		Viewport(Viewport&& other) = default;

		Viewport& operator = (const Viewport& other) = default;
		Viewport& operator = (Viewport&& other) = default;

		~Viewport() noexcept = default;

		void setSize(const std::pair<ZtInt, ZtInt>& newSize);
		const std::pair<ZtInt, ZtInt>& getSize() const;

		void setPosition(const std::pair<ZtInt, ZtInt>& newPosition);
		const std::pair<ZtInt, ZtInt>& getPosition() const;

		void apply();

	protected:

		std::pair<ZtInt, ZtInt> size;
		std::pair<ZtInt, ZtInt> position;

	};

}

#endif // ZINET_USE_OPENGL