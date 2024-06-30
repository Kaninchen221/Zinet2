#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtTypes.hpp"

namespace zt::rhi
{

	class ZINET_RHI_API OpenGLViewport
	{
	public:
		OpenGLViewport() = default;
		OpenGLViewport(const OpenGLViewport& other) = default;
		OpenGLViewport(OpenGLViewport&& other) = default;

		OpenGLViewport& operator = (const OpenGLViewport& other) = default;
		OpenGLViewport& operator = (OpenGLViewport&& other) = default;

		~OpenGLViewport() noexcept = default;

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