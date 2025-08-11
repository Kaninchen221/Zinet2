#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	struct DrawInfo;

	
	class ZINET_VULKAN_RENDERER_API VulkanRenderer : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VulkanRenderer");

	public:

		VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer& other) = default;
		VulkanRenderer(VulkanRenderer&& other) = default;
		~VulkanRenderer() noexcept = default;

		VulkanRenderer& operator = (const VulkanRenderer& other) noexcept = default;
		VulkanRenderer& operator = (VulkanRenderer&& other) noexcept = default;

		bool init(wd::Window& window);

		void deinit();

		bool beginFrame();

		bool createPipeline(DrawInfo& drawInfo);
		
		void draw(const DrawInfo& drawInfo);

		bool submit();

		bool endFrame();

		const auto& getRendererContext() const { return rendererContext; }
		auto& getRendererContext() { return rendererContext; }

		const auto& getGraphicsPipeline() const { return graphicsPipeline; }
		auto& getGraphicsPipeline() { return graphicsPipeline; }

	protected:

		RendererContext rendererContext;
		GraphicsPipeline graphicsPipeline;

		static void WindowResizedCallback(void* userPointer, const Vector2i& size);

	};
}