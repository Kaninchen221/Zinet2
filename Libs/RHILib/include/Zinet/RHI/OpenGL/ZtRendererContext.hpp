#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/RHI/OpenGL/ZtViewport.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::rhi::opengl
{

	class ZINET_RHI_API RendererContext
	{

	public:

		RendererContext() = default;
		RendererContext(const RendererContext& other) = default;
		RendererContext(RendererContext&& other) = default;

		RendererContext& operator = (const RendererContext& other) = default;
		RendererContext& operator = (RendererContext&& other) = default;

		~RendererContext() noexcept = default;

		const Viewport& getViewport() const { return viewport; }
		Viewport& getViewport() { return viewport; }

		void setClearColor(const Vector4f& newClearColor);
		const Vector4f& getClearColor() const { return clearColor; }

	protected:

		Viewport viewport;
		Vector4f clearColor{};
	};

}

#endif // ZINET_USE_OPENGL