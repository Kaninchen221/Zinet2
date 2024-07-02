#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/RHI/OpenGL/ZtRendererContext.hpp"

namespace zt::rhi::opengl
{

	class ZINET_RHI_API Renderer
	{

	public:

		Renderer() = default;
		Renderer(const Renderer& other) = default;
		Renderer(Renderer&& other) = default;

		Renderer& operator = (const Renderer& other) = default;
		Renderer& operator = (Renderer&& other) = default;

		~Renderer() noexcept = default;

		void startRenderingFrame();
		void endRenderingFrame();

		RendererContext& getRendererContext() { return rendererContext; }
		const RendererContext& getRendererContext() const { return rendererContext; }

	protected:

		RendererContext rendererContext;

	};

}

#endif // ZINET_USE_OPENGL