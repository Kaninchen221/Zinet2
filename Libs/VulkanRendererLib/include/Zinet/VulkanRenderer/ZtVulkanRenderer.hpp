#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	struct DrawInfo;

	ZT_REFLECT_CLASS()
	class ZINET_VULKAN_RENDERER_API VulkanRenderer : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VulkanRenderer");

	public:

		bool init(wd::Window& window) ZINET_API_POST;

		void deinit() ZINET_API_POST;

		bool beginFrame() ZINET_API_POST;

		bool createPipeline(DrawInfo& drawInfo) ZINET_API_POST;
		
		void draw(const DrawInfo& drawInfo) ZINET_API_POST;

		bool submit() ZINET_API_POST;

		bool endFrame() ZINET_API_POST;

		const auto& getRendererContext() const ZINET_API_POST { return rendererContext; }
		auto& getRendererContext() ZINET_API_POST { return rendererContext; }

		const auto& getGraphicsPipeline() const ZINET_API_POST { return graphicsPipeline; }
		auto& getGraphicsPipeline() ZINET_API_POST { return graphicsPipeline; }

	protected:

		RendererContext rendererContext;
		GraphicsPipeline graphicsPipeline;

		static void WindowResizedCallback(void* userPointer, const Vector2i& size);

	};
}